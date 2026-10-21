/*
 * A236058  Number of (n+1)X(1+1) 0..2 arrays colored with the difference of
 *          the upper median and the minimum in each 2X2 subblock.
 * https://oeis.org/A236058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-3) + 173*a(n-4) + 268*a(n-5) + 1216*a(n-6) +
 *            1260*a(n-7) + 2128*a(n-8) + 1296*a(n-9) - 3968*a(n-10) -
 *            1024*a(n-11) - 12288*a(n-12) + 256*a(n-13) - 6144*a(n-14)
 *            Initial terms: a(1)=81, a(2)=360, a(3)=1616, a(4)=7016,
 *            a(5)=31616, a(6)=138988, a(7)=620552, a(8)=2734804,
 *            a(9)=12176064,...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, but we only use idx_t */

seq_t a(idx_t n)
{
    /* Cached values, zero‐indexed.  cache[0] is unused. */
    static seq_t cache[20001];
    static idx_t cached = 0;       /* highest index computed */

    /* Initial values a(1)..a(14).  cache[0] is not used. */
    static const seq_t init[15] = {
        0ULL,            /* dummy for index 0 */
        81ULL,
        360ULL,
        1616ULL,
        7016ULL,
        31616ULL,
        138988ULL,
        620552ULL,
        2734804ULL,
        12176064ULL,
        53818764ULL,
        239177944ULL,
        1058421796ULL,
        4698785664ULL,
        20810853180ULL
    };

    /* Coefficients for lags 3..14 (index 0..2 unused). */
    static const int coeff[15] = {
        0, 0, 0,          /* 0,1,2 unused */
        16, 173, 268,     /* 3,4,5 */
        1216, 1260, 2128, /* 6,7,8 */
        1296,             /* 9 */
        -3968, -1024, -12288, /* 10,11,12 */
        256,              /* 13 */
        -6144             /* 14 */
    };

    /* For very small n we can return directly. */
    if (n <= 14U && n >= 1U) {
        return init[n];
    }

    /* Initialise cache for first 14 terms on first use. */
    if (cached < 14U) {
        for (idx_t i = 1U; i <= 14U; ++i) {
            cache[i] = init[i];
        }
        cached = 14U;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cached + 1U; i <= n; ++i) {
        seq_t sum = 0;
        /* apply recurrence for lags 3..14 */
        for (int k = 3; k <= 14; ++k) {
            int c = coeff[k];
            seq_t term = cache[i - k];
            if (c > 0) {
                sum += (seq_t)c * term;
            } else { /* negative coefficient */
                sum -= (seq_t)(-c) * term;
            }
        }
        cache[i] = sum;
    }
    cached = n;
    return cache[n];
}
