/*
 * A099180  Primes p which are greater than p reversed.
 * https://oeis.org/A099180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   is_prime, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099180 – Primes p with p > reverse(p) (decimal) */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *c = NULL;
    static idx_t  found = 0;
    static idx_t  cur   = 1;
    static size_t cap   = 0;

    /* generate term by term until we have at least n entries */
    while (found < n)
    {
        cur++;

        /* skip composites */
        if (!is_prime(cur))
            continue;

        seq_t rev_cur = (seq_t)reverse_digits_idx(cur);
        if (rev_cur >= (seq_t)cur)
            continue;    /* condition p > reverse(p) not satisfied */

        /* store valid prime */
        ensure_capacity(found + 1, &cap, (void **)&c, sizeof(seq_t));
        c[found++] = (seq_t)cur;
    }

    /* OEIS uses 1‑based indexing */
    return c[n - 1];
}
