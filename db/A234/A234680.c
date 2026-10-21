/*
 * A234680  Number of (n+1) X (7+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) +47*a(n-2) -200*a(n-3) -901*a(n-4)
 *          +4204*a(n-5) +8957*a(n-6) -48440*a(n-7) -47194*a(n-8)
 *          +334096*a(n-9)+101948*a(n-10) -1410080*a(n-11)+170136*a(n-12)
 *          +3549696*a(n-13)-1446912*a(n-14)-4861440*a(n-15)
 *          +2954880*a(n-16)+2764800*a(n-17)-2073600*a(n-18).
 * Initial values:
 * a(1)=164416, a(2)=243164, a(3)=354856, a(4)=639440, a(5)=1109272,
 * a(6)=2354408, a(7)=4736536, a(8)=11294552, a(9)=25494136,
 * a(10)=66161144, a(11)=162985816, a(12)=451913720, a(13)=1189756792,
 * a(14)=3483132728, a(15)=9648855256, a(16)=29560946552,
 * a(17)=85169950456, a(18)=270999391544. */

#include <stdlib.h>

/* Initial 18 terms (1‑based indices). */
static const seq_t init[18] = {
    164416ULL, 243164ULL, 354856ULL, 639440ULL, 1109272ULL,
    2354408ULL, 4736536ULL, 11294552ULL, 25494136ULL,
    66161144ULL, 162985816ULL, 451913720ULL, 1189756792ULL,
    3483132728ULL, 9648855256ULL, 29560946552ULL, 85169950456ULL,
    270999391544ULL
};

/* Corresponding coefficients (a(1) is considered a(0) in the recurrence). */
static const int coeff[18] = {
    4, 47, -200, -901, 4204, 8957, -48440, -47194, 334096,
    101948, -1410080, 170136, 3549696, -1446912, -4861440,
    2954880, 2764800, -2073600
};

/* Cache of computed values: cache[i] holds a(i+1) for i >= 0. */
static seq_t *cache = NULL;
/* Number of terms stored in 'cache'. */
static idx_t computed = 0;

/* Return the n-th term (n >= 1).  For n == 0 the function returns 0
 * as the OEIS offset starts at 1. */
seq_t a(idx_t n) {
    if (n == 0)          /* outside the defined range */
        return 0;

    /* Grow the cache if needed. */
    if (n > computed) {
        size_t newSize = (size_t)n;
        seq_t *newCache = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!newCache)   /* out of memory – return 0 as a sentinel */
            return 0;
        cache = newCache;

        /* Compute missing terms. */
        for (idx_t i = computed; i < n; ++i) {
            if (i < 18) {
                cache[i] = init[i];
            } else {
                seq_t sumPos = 0, sumNeg = 0;
                for (int j = 1; j <= 18; ++j) {
                    int c = coeff[j - 1];
                    seq_t val = cache[i - j];
                    seq_t prod = (seq_t)((c >= 0 ? c : -c)) * val;
                    if (c >= 0)
                        sumPos += prod;
                    else
                        sumNeg += prod;
                }
                cache[i] = sumPos - sumNeg;
            }
        }
        computed = n;
    }

    return cache[n - 1];
}
