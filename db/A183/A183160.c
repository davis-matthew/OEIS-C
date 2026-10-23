/*
 * A183160  a(n) = Sum_{k=0..n} C(n+k,n-k)*C(2*n-k,k).
 * https://oeis.org/A183160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} C(n+k,n-k) * C(2*n-k,k)
 *
 * The sequence satisfies the 3‑term recurrence
 *   320*n*(2*n-1)*a(n) =
 *     8*(346*n^2 + 79*n - 327) * a(n-1) +
 *     6*(1688*n^2 - 6241*n + 5981) * a(n-2) +
 *     261*(3*n-7)*(3*n-5) * a(n-3)
 * for n ≥ 3 with initial values a(0)=1, a(1)=2, a(2)=11.
 *
 * The implementation memoises results in a static array.
 * For 64‑bit seq_t values it uses unsigned __int128 for
 * intermediate arithmetic to avoid overflow; for larger
 * arbitrary‑precision seq_t types it performs the
 * calculations with seq_t operators, which are defined
 * for the big‑integer builds.
 *
 * This function has external linkage and can be compiled
 * with either C11 or C++17.
 *
 * ------------------------------------------------------------------
 */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache can hold indices up to n */
    if (n + 1 > cache_size) {
        seq_t *new_cache = seq_alloc_array(n + 1);
        if (cache) free_seq_array(cache);
        cache = new_cache;
        cache_size = n + 1;

        /* initialise first three terms */
        cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)2;
        if (cache_size > 2) cache[2] = (seq_t)11;

        /* compute remaining terms up to n */
        for (idx_t i = 3; i < cache_size; ++i) {
            /* recurrence coefficients */
            if (sizeof(seq_t) <= sizeof(uint64_t)) {          /* 64‑bit path */
                __int128 ii = i;
                __int128 denom = (__int128)320 * ii * (2 * ii - 1);

                __int128 m1 = 8 * ((346 * ii * ii) + (79 * ii) - 327);
                __int128 m2 = 6 * ((1688 * ii * ii) - (6241 * ii) + 5981);
                __int128 m3 = 261 * ((3 * ii - 7) * (3 * ii - 5));

                __int128 sum = m1 * ( (__int128)cache[i-1] )
                             + m2 * ( (__int128)cache[i-2] )
                             + m3 * ( (__int128)cache[i-3] );

                cache[i] = (seq_t)(sum / denom);
            } else {                                         /* arbitrary‑precision path */
                seq_t ii = (seq_t)i;

                /* denominator as big integer */
                seq_t denom = 320 * ii * (2 * ii - 1);

                /* coefficient multipliers as big integers */
                seq_t m1 = 8 * ( (346 * ii * ii) + (79 * ii) - 327 );
                seq_t m2 = 6 * ( (1688 * ii * ii) - (6241 * ii) + 5981 );
                seq_t m3 = 261 * ( (3 * ii - 7) * (3 * ii - 5) );

                /* recurrence computation */
                cache[i] = (m1 * cache[i-1] + m2 * cache[i-2] + m3 * cache[i-3]) / denom;
            }
        }
    }

    return cache[n];
}
