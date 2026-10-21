/*
 * A099180  Primes p which are greater than p reversed.
 * https://oeis.org/A099180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   ensure_capacity, is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099180 – Primes p with p > reverse(p) (decimal) */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/rev.h"
#include "../utils/ensure_capacity.h"

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

        seq_t rev_cur = (seq_t)rev(cur);
        if (rev_cur >= (seq_t)cur)
            continue;    /* condition p > reverse(p) not satisfied */

        /* store valid prime */
        ensure_capacity(found + 1, &cap, (void **)&c, sizeof(seq_t));
        c[found++] = (seq_t)cur;
    }

    /* OEIS uses 1‑based indexing */
    return c[n - 1];
}
