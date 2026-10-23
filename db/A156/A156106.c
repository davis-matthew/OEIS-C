/*
 * A156106  Expansion of F(1/3,2/3;1/2;27*x/2) / F(1/3,-1/3;-1/2;27*x/2).
 * https://oeis.org/A156106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(2*n-1)*(n-2)*a(n) + (-72*(n-3)^2-171*n+420)*a(n-1) +
 *            (297*(n-3)^2+675*n-1674)*a(n-2) - 81*(3*n-5)*(3*n-7)*a(n-3) = 0
 *            Initial values: a(0)=1, a(1)=3, a(2)=15, a(3)=99
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* cached terms */
    static idx_t  cap    = 0;      /* allocated capacity */
    static idx_t  size   = 0;      /* number of computed terms */

    /* Ensure capacity at least max(n+1,4) to hold the base cases. */
    if (n + 1 > cap) {
        idx_t newcap = (n + 1 > 4) ? (n + 1) : 4;
        seq_t *tmp = seq_alloc_array(newcap);
        if (!tmp) return 0;          /* allocation failure */

        /* Copy old values if any and free the old array. */
        for (idx_t j = 0; j < size; ++j)
            tmp[j] = cache[j];
        if (cache)
            seq_free_array(cache);

        cache = tmp;
        cap   = newcap;
    }

    /* Initialize the first four known terms when needed. */
    if (size < 4) {
        seq_t init[4] = {(seq_t)1, (seq_t)3, (seq_t)15, (seq_t)99};
        size_t upto  = (size_t)cap < 4 ? (size_t)cap : 4;
        for (size_t j = 0; j < upto; ++j)
            cache[j] = init[j];
        if (size < 4) size = 4;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = size; i <= n; ++i) {
        /* Ignore i < 4 because they are already initialized. */
        if (i < 4) continue;

        int64_t i64  = (int64_t)i;
        int64_t m3   = i64 - 3;
        int64_t m3sq = m3 * m3;

        /* Coefficients from the recurrence. */
        int64_t c1 = -72 * m3sq - 171 * i64 + 420;                            /* a(i-1) coeff */
        int64_t c2 = 297 * m3sq + 675 * i64 - 1674;                          /* a(i-2) coeff */
        int64_t c3 = -81 * (3 * i64 - 5) * (3 * i64 - 7);                     /* a(i-3) coeff */

        if (sizeof(seq_t) <= 8) {  /* fast integer case (uint64_t or smaller) */
            __int128 term1 = (__int128)cache[i-1] * (__int128)c1;
            __int128 term2 = (__int128)cache[i-2] * (__int128)c2;
            __int128 term3 = (__int128)cache[i-3] * (__int128)c3;
            __int128 numer = -(term1 + term2 + term3);
            uint64_t denom = 2ULL * (2ULL * (uint64_t)i64 - 1ULL) *
                            (uint64_t)(i64 - 2);
            cache[i] = (seq_t)(numer / denom);
        } else {                    /* arbitrary‑precision case */
            seq_t t1 = cache[i-1] * (seq_t)c1;
            seq_t t2 = cache[i-2] * (seq_t)c2;
            seq_t t3 = cache[i-3] * (seq_t)c3;
            seq_t numer = -(t1 + t2 + t3);
            seq_t denom = (seq_t)(2ULL * (2ULL * (uint64_t)i64 - 1ULL) *
                                 (uint64_t)(i64 - 2));
            cache[i] = numer / denom;
        }
    }

    size = n + 1;
    return cache[n];
}
