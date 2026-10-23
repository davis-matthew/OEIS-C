/*
 * A169073  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169073 – Number of reduced words of length n in the Coxeter group
 *  with 12 generators and relations (S_i)^2 = (S_i S_j)^25 = I.
 *
 *  The generating function is
 *      (t^25 + 2*t^24 + 2*t^23 + … + 2*t + 1)
 *      ---------------------------------------
 *      1 - 10*t - 10*t^2 - … - 10*t^24 + 55*t^25
 *
 *  Hence, for all n ≥ 0
 *      a(n) = 10*a(n-1) + 10*a(n-2) + … + 10*a(n-24)
 *             – 55*a(n-25) + rhs(n)
 *
 *  with
 *      rhs(0) = 1,
 *      rhs(n) = 2   for 1 ≤ n ≤ 24,
 *      rhs(25) = 1,
 *      rhs(n) = 0   for n ≥ 26.
 *
 *  The following implementation uses an O(n) recurrence with
 *  memoisation.  All arithmetic is performed with the unsigned
 *  type seq_t and is valid for 8‑, 16‑, 32‑, 64‑, 128‑bit builds
 *  as well as an arbitrary‑precision build.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;   /* cache[0 … n] */
static size_t  cap   = 0;     /* allocated capacity */
static size_t  len   = 0;     /* number of stored values */

/* Ensure the cache can hold at least 'need' elements.  */
static void grow_cache(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap ? cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cap   = new_cap;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Make sure we have computed terms up to index n. */
    if ((size_t)n >= len) {
        grow_cache((size_t)n + 1);
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t sum = 0;

            /* Add 10*a(i-k) for k = 1 … 24 when i ≥ k. */
            for (idx_t k = 1; k <= 24; ++k) {
                if (i >= k) {
                    sum += cache[(size_t)(i - k)] * (seq_t)10;
                }
            }

            /* Subtract 55*a(i-25) when i ≥ 25. */
            if (i >= 25) {
                sum -= cache[(size_t)(i - 25)] * (seq_t)55;
            }

            /* Add the RHS term. */
            seq_t rhs = 0;
            if (i == 0) {
                rhs = 1;
            } else if (i <= 24) {
                rhs = 2;
            } else if (i == 25) {
                rhs = 1;
            } /* otherwise rhs remains 0 */

            sum += rhs;
            cache[(size_t)i] = sum;
        }
        len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
