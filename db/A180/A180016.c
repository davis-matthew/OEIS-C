/*
 * A180016  Partial sums of number of n-step closed paths on hexagonal lattice
 *          A002898.
 * https://oeis.org/A180016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Sum_{i=0..n} A002898(i) Recurrence: for n ≥ 4 n²·a(n) =
 *            (2n−1)n·a(n−1) + (n−1)(23n−24)·a(n−2) + 12(n−4)(n−1)·a(n‑3) −
 *            36(n−2)(n−1)·a(n‑4) Initial terms: a(0)=1, a(1)=1, a(2)=7,
 *            a(3)=19
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  last_computed = 0;
    static bool   initialized = false;

    /* initialise the cache with the first four values once */
    if (!initialized) {
        /* request space for indices 0..3 */
        ensure_cache(4, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)19;
        last_computed   = 3;
        initialized     = true;
    }

    /* answer trivial requests directly */
    if (n <= last_computed) {
        return cache[n];
    }

    /* enlarge cache if necessary (ensure at least n+1 slots) */
    if (n + 1 > (idx_t)cache_cap) {
        ensure_cache(n + 1, &cache, &cache_cap);
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* previous values */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        seq_t ni   = (seq_t)i;
        seq_t nim1 = (seq_t)(i - 1);
        seq_t nim2 = (seq_t)(i - 2);
        seq_t nim4 = (seq_t)(i - 4);

        /* build the numerator of the recurrence */
        seq_t term1 = ((seq_t)2 * ni - (seq_t)1) * ni * a1;
        seq_t term2 = (nim1 * ((seq_t)23 * ni - (seq_t)24)) * a2;
        seq_t term3 = ((seq_t)12 * nim4 * nim1) * a3;
        seq_t term4 = ((seq_t)36 * nim2 * nim1) * a4;

        seq_t num = term1 + term2 + term3 - term4;
        seq_t denom = ni * ni;      /* n² */

        cache[i] = num / denom;
    }

    last_computed = n;
    return cache[n];
}
