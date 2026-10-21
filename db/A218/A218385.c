/*
 * A218385  (n-2)!/n when an integer.
 * https://oeis.org/A218385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A218385
   a(n) = (c-2)! / c  where c is the nth composite number >= 6
   (a(1) corresponds to c = 6, a(2) to c = 8, a(3) to c = 9, …) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* ==============================================================
   Memoisation state
   ============================================================== */
static idx_t  computed     = 0;  /* number of terms already computed   */
static idx_t  last_comp    = 5;  /* last composite processed           */
static idx_t  fact_idx     = 0;  /* current factorial index (fact = fact_idx!) */
static seq_t  fact         = 1;  /* factorial value of fact_idx        */

/* array for cached terms */
static seq_t *values      = NULL;
static size_t values_cap  = 0;
static size_t values_len  = 0;

/* ensure the values array can hold at least \"needed\" terms */
static void ensure_capacity(size_t needed)
{
    if (needed <= values_cap)
        return;

    size_t new_cap = values_cap ? values_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
    /* on failure the program aborts – an out‑of‑memory condition
       is considered unrecoverable for this project */
    values = tmp;
    values_cap = new_cap;
}

/* ==============================================================
   A218385 sequence
   ============================================================== */
seq_t a(idx_t n)
{
    if (n == 0)                /* safety for offset 1 */
        return 0;

    /* return cached value if it already exists */
    if (n <= computed)
        return values[n - 1];

    /* compute missing terms up to the requested index */
    while (computed < n) {
        /* find next composite number >= 6 */
        idx_t c = last_comp + 1;
        while (c < 6 || is_prime(c))
            ++c;

        /* extend the factorial to (c-2)! */
        idx_t target = c - 2;
        for (idx_t i = fact_idx + 1; i <= target; ++i)
            fact = fact * (seq_t)i;
        fact_idx = target;

        /* compute the term and store it */
        seq_t val = fact / (seq_t)c;

        ensure_capacity(values_len + 1);
        values[values_len++] = val;
        ++computed;
        last_comp = c;
    }

    return values[n - 1];
}
