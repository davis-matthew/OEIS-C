/*
 * A109034  First differences of A109033.
 * https://oeis.org/A109034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence *
 *            (n+1)*a(n)+4*(-2*n+1)*a(n-1)+16*(n-2)*a(n-2)+4*(-2*n+7)*a(n-3)=0,
 *            * with initial values a(0)=1, a(1)=0, a(2)=1. * The recurrence
 *            also gives a(3)=4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul128.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of previously computed terms */
    static idx_t cap   = 0;          /* currently allocated size of cache   */
    static idx_t stored = 0;         /* number of terms stored              */

    /* Initialise cache for the first four terms (0‑3). */
    if (stored == 0) {
        cap = 4; /* minimal capacity for indices 0…3 */
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(0) */
        cache[1] = (seq_t)0;      /* a(1) */
        cache[2] = (seq_t)1;      /* a(2) */
        cache[3] = (seq_t)4;      /* a(3) from the recurrence */
        stored = 4;
    }

    /* Grow the cache if n is outside the current bounds. */
    if (n + 1 > cap) {
        idx_t newcap = cap ? cap : 1;
        while (newcap <= n) newcap <<= 1;  /* double until big enough */
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute missing terms up to n. */
    if (n >= stored) {
        for (idx_t k = stored; k <= n; ++k) {
            if (k <= 3) continue;    /* base terms already set */

            /* Compute each part of the numerator using mul128 to avoid
               intermediate overflow on large seq_t types. */
            seq_t term1 = mul128((seq_t)(4ULL * (2*k - 1)), cache[k-1]);
            seq_t term2 = mul128((seq_t)(16ULL * (k-2)),     cache[k-2]);
            seq_t term3 = mul128((seq_t)(4ULL * (2*k - 7)),  cache[k-3]);

            seq_t numerator = term1 + term3 - term2;
            cache[k] = numerator / (seq_t)(k + 1);  /* division by (k+1) */
        }
        stored = n + 1;
    }

    return cache[n];
}
