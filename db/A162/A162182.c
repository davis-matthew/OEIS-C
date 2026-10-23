/*
 * A162182  Number of reduced words of length n in the Weyl group B_44.
 * https://oeis.org/A162182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{44} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the number of elements of length n in the Weyl group B_44. */

#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 big_t; /* internal type big enough for intermediate sums */

seq_t a(idx_t n)
{
    static bool init = false;
    static big_t *dp = NULL;          /* coefficients array */
    static size_t dp_len = 0;         /* current maximum degree stored */
    const size_t M = 44;              /* rank of the Weyl group B_M */
    const size_t MAXLEN = M * M;      /* 44^2 = 1936, maximum degree of the product */

    if (!init) {
        /* initial allocation for degree 0 */
        dp = (big_t*)malloc((1) * sizeof(big_t));
        dp[0] = 1;
        dp_len = 0;

        /* multiply successively by each (1 + x + … + x^{2k-1}) */
        for (size_t k = 1; k <= M; ++k) {
            size_t r = 2 * k;                 /* range of the factor: 0 … (2k-1) */
            size_t new_len = dp_len + r - 1;   /* new maximum degree */

            big_t *newdp = (big_t*)malloc((new_len + 1) * sizeof(big_t));
            for (size_t i = 0; i <= new_len; ++i) newdp[i] = 0;

            /* use a difference array to perform the convolution in O(new_len) */
            big_t *diff = (big_t*)calloc((new_len + 2), sizeof(big_t));

            for (size_t i = 0; i <= dp_len; ++i) {
                if (dp[i] == 0) continue;
                diff[i] += dp[i];
                diff[i + r] -= dp[i];
            }

            big_t acc = 0;
            for (size_t i = 0; i <= new_len; ++i) {
                acc += diff[i];
                newdp[i] = acc;
            }

            free(dp);
            free(diff);
            dp = newdp;
            dp_len = new_len;
        }
        init = true;
    }

    if (n <= dp_len) {
        return (seq_t)dp[n];
    } else {
        /* beyond the degree of the polynomial the coefficient is zero */
        return (seq_t)0;
    }
}
