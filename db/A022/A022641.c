/*
 * A022641  Expansion of Product_{m>=1} (1 + m*q^m)^13.
 * https://oeis.org/A022641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in product_{m>=1}(1 + m*q^m)^13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 38, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming.  For each m (1≤m≤n)
   we convolve the current sequence with the polynomial
   (1 + m*q^m)^13 = Σ_{k=0}^{13} C(13,k) * m^k * q^{m*k}.  */
/* The code is safe for both plain-integer and arbitrary-precision
   seq_t types.  In C++ builds it uses std::vector so that objects are
   default‑constructed; in C builds it uses malloc. */

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>

static const unsigned int binom13[14] = {
    1, 13, 78, 286, 715, 1287, 1716, 1716,
    1287, 715, 286, 78, 13, 1
};

seq_t a(idx_t n)
{
    /* Allocate space for the DP array (0..n).  We keep it as a raw
       pointer for C builds, and as a std::vector for C++ builds. */
    size_t len = (size_t)n + 1;
    seq_t *dp = NULL;
#ifdef __cplusplus
    std::vector<seq_t> dp_vec(len, 0);
    dp = dp_vec.data();
#else
    dp = (seq_t*)malloc(len * sizeof(seq_t));
    if (!dp) return 0;   /* allocation failed */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
#endif

    dp[0] = 1;   /* empty product contributes 1 */

    /* Main DP: iterate over part sizes m. */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t coeff[14];
        /* coeff[k] = C(13,k) * m^k */
        seq_t powk = 1;          /* m^0 */
        coeff[0] = 1;            /* unused */
        for (int k = 1; k <= 13; ++k) {
            powk = powk * (seq_t)m;                    /* m^k */
            coeff[k] = (seq_t)binom13[k] * powk;
        }

        /* Convolution: iterate i from n down to 0 so that each new
           coefficient uses only old values from the same m. */
        for (size_t i = len - 1; i != (size_t)-1; --i) {
            for (int k = 1; k <= 13; ++k) {
                int idx = (int)i - (int)(m * (idx_t)k);
                if (idx < 0) break;
                dp[i] += dp[idx] * coeff[k];
            }
            if (i == 0) break;  /* avoid underflow of size_t */
        }
    }

    seq_t result = dp[(size_t)n];
#ifndef __cplusplus
    free(dp);
#endif
    return result;
}
