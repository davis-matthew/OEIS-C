/*
 * A161987  Number of reduced words of length n in the Weyl group B_32.
 * https://oeis.org/A161987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the product ∏_{k=1..32} (1 + x + …
 *            + x^(2k-1) ) = ∏_{k=1..32} (1 - x^(2k))/(1-x)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This product is a polynomial of degree 1055.  The sequence is
   a(n) = number of reduced words of length n in the Weyl group B_32. */

#include <stdbool.h>
#include <stddef.h>

#define MAX_DEGREE 1055          /* maximum n with a(n) ≠ 0 for B_32 */
#define POLY_SIZE (MAX_DEGREE+1) /* array length: indices 0..1055 */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t dp[POLY_SIZE];   /* current convolution result */
    static seq_t tmp[POLY_SIZE];  /* temporary buffer for next step */

    if (!initialized) {
        /* initialise dp[0] = 1, others 0 */
        for (size_t i = 0; i < POLY_SIZE; ++i) dp[i] = 0;
        dp[0] = 1;
        size_t curr_max = 0;      /* current maximum index with non‑zero entry */

        for (idx_t k = 1; k <= 32; ++k) {
            size_t len = 2 * k;                    /* factor length = 2k */
            size_t new_max = curr_max + len - 1;    /* new maximum index */

            /* zero temporary buffer up to new_max */
            for (size_t i = 0; i <= new_max; ++i) tmp[i] = 0;

            /* convolution dp * (1 + x + … + x^(len-1)) */
            for (size_t i = 0; i <= curr_max; ++i) {
                seq_t val = dp[i];
                if (val == 0) continue;
                size_t maxj = len - 1;
                if (i + maxj > new_max) maxj = new_max - i;
                for (size_t j = 0; j <= maxj; ++j) {
                    tmp[i + j] += val;
                }
            }

            /* copy result back to dp */
            for (size_t i = 0; i <= new_max; ++i) dp[i] = tmp[i];

            curr_max = new_max;
        }

        initialized = true;
    }

    if (n > MAX_DEGREE) return 0;
    return dp[(size_t)n];
}
