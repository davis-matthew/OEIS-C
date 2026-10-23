/*
 * A161956  Number of reduced words of length n in the Weyl group B_28.
 * https://oeis.org/A161956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in * ∏_{k=1}^{28} (1 + x + … +
 *            x^{2k-1}) (1) * (same as ∏_{k=1}^{28} (1 - x^{2k})/(1 - x)). *
 *            The product is a polynomial of degree 28² = 784. * *
 *            Computation: dynamic programming. * dp[s] = number of ways to
 *            obtain sum s using the first t factors. * Initially dp[0] =
 *            1,...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 44, u128 n <= 784,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>    /* for malloc/free */

enum { M = 28, MAX_DEG = M * M };   /* 28² = 784 */

static seq_t dp[MAX_DEG + 1];
static seq_t tmp[MAX_DEG + 1];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (n > (idx_t)MAX_DEG) return 0;   /* polynomial degree is 784 */

    if (!initialized) {
        /* initial state: only the empty selection is possible */
        for (size_t i = 0; i <= MAX_DEG; ++i) {
            dp[i] = 0;
        }
        dp[0] = 1;

        /* iterate over the 28 factors */
        for (int k = 1; k <= M; ++k) {
            int block_len = 2 * k;            /* 1 + … + x^{2k-1}  */
            size_t new_len = MAX_DEG + 1;     /* we keep the full range */

            /* start with the old dp array */
            for (size_t i = 0; i < new_len; ++i)
                tmp[i] = dp[i];

            /* convolve: for each existing sum add all possible positive offsets */
            for (size_t s = 0; s <= MAX_DEG; ++s) {
                seq_t val = dp[s];
                if (val == 0) continue;
                size_t limit = (size_t)block_len;
                for (size_t add = 1; add < limit; ++add) {
                    size_t idx = s + add;
                    if (idx > MAX_DEG) break;
                    tmp[idx] += val;
                }
            }

            /* copy back to dp for the next iteration */
            for (size_t i = 0; i <= MAX_DEG; ++i)
                dp[i] = tmp[i];
        }
        initialized = true;
    }

    return dp[(size_t)n];
}
