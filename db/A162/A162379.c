/*
 * A162379  Number of reduced words of length n in the Weyl group D_32.
 * https://oeis.org/A162379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Weyl group
 *            D_32. Compute a(n) by truncating the product of polynomials f(m)
 *            = 1 + x + ... + x^{m-1} for m = 32, 2, 4, ..., 62, using
 *            symmetry of the resulting palindromic polynomial of degree 992
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int MAX_DEG = 992;   /* degree of the growth series polynomial */
static const int HALF_DEG = MAX_DEG / 2;

/* Helper list of exponents for the factors f(m) */
static const int exps[32] = {
    32,  2,  4,  6,  8, 10, 12, 14, 16, 18,
   20, 22, 24, 26, 28, 30, 32, 34, 36, 38,
   40, 42, 44, 46, 48, 50, 52, 54, 56, 58,
   60, 62
};

seq_t a(idx_t n)
{
    /* Quick failure for out-of-range indices */
    if (n > (idx_t)MAX_DEG) return (seq_t)0;

    /* Exploit palindromicity: a(n) = a(MAX_DEG - n) */
    if (n > (idx_t)HALF_DEG) n = (idx_t)MAX_DEG - n;

    idx_t N = n;
    size_t len = (size_t)N + 1;

    /* Allocate DP arrays */
    seq_t *dp   = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp || !next) {
        /* In case of allocation failure return 0 */
        if (dp) free(dp);
        if (next) free(next);
        return (seq_t)0;
    }

    /* Initialize DP: only sum 0 is achievable initially */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Iterate over all 32 factors */
    for (int idx = 0; idx < 32; ++idx) {
        int m = exps[idx];           /* length of the factor (m terms) */
        int max_inc = m - 1;         /* maximum offset for this factor */

        /* Zero the next array */
        for (size_t i = 0; i < len; ++i) next[i] = 0;

        /* Convolution with the current factor */
        for (size_t t = 0; t < len; ++t) {
            seq_t val = dp[t];
            if (!val) continue;
            for (int s = 0; s <= max_inc && t + s < len; ++s) {
                next[t + s] += val;
            }
        }
        /* Swap dp and next for next iteration */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    seq_t result = dp[N];

    free(dp);
    free(next);
    return result;
}
