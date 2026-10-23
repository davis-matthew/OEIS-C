/*
 * A161573  Number of reduced words of length n in the Weyl group A_30.
 * https://oeis.org/A161573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{30} (1+x+…+x^k) * This
 *            equals the number of solutions of * j_1 + j_2 + … + j_30 = n
 *            with 0 ≤ j_k ≤ k
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 41, u128 n <= 465,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_M 30
#define MAX_SUM ((MAX_M*(MAX_M+1))/2)   // 465

seq_t a(idx_t n)
{
    /* Return 0 for indices beyond the maximum possible sum. */
    if (n > MAX_SUM)
        return 0;

    /* Static cache of coefficients, computed once. */
    static seq_t coeff[MAX_SUM + 1];
    static bool computed = false;

    if (!computed) {
        /* Initialize with the unit polynomial 1. */
        for (idx_t i = 0; i <= MAX_SUM; ++i)
            coeff[i] = 0;
        coeff[0] = 1;

        /* Iterate over each factor (1 + x + ... + x^k). */
        for (idx_t k = 1; k <= MAX_M; ++k) {
            seq_t tmp[MAX_SUM + 1];
            /* tmp will accumulate the new coefficients for this step. */
            for (idx_t i = 0; i <= MAX_SUM; ++i)
                tmp[i] = 0;

            for (idx_t i = 0; i <= MAX_SUM; ++i) {
                seq_t ci = coeff[i];
                if (ci == 0)
                    continue;
                /* Add contributions for j = 0 .. k. */
                for (idx_t j = 0; j <= k && i + j <= MAX_SUM; ++j) {
                    tmp[i + j] += ci;
                }
            }
            /* Copy back to coeff for next iteration. */
            for (idx_t i = 0; i <= MAX_SUM; ++i)
                coeff[i] = tmp[i];
        }
        computed = true;
    }

    return coeff[n];
}
