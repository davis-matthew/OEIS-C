/*
 * A162178  Number of reduced words of length n in the Weyl group B_41.
 * https://oeis.org/A162178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the polynomial ∏_{k=1}^{41} (1 + x
 *            + … + x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define M 41
#define D (M*M)          /* maximum degree = 1681 */

static seq_t coeff[D + 1];
static bool initialized = false;

static void compute_cache(void)
{
    static seq_t temp[D + 1];
    size_t cur_deg = 0;

    /* initialize as 1 */
    coeff[0] = 1;
    for (size_t i = 1; i <= D; ++i) coeff[i] = 0;

    for (size_t k = 1; k <= M; ++k) {
        size_t exp = 2 * k - 1;          /* degree of factor */
        size_t new_deg = cur_deg + exp;

        /* zero temp array up to new_deg */
        for (size_t i = 0; i <= new_deg; ++i) temp[i] = 0;

        /* convolution with a window of ones of length exp+1 */
        for (size_t i = 0; i <= cur_deg; ++i) {
            seq_t val = coeff[i];
            if (val == 0) continue;
            for (size_t j = 0; j <= exp; ++j) {
                temp[i + j] += val;
            }
        }

        /* copy back */
        for (size_t i = 0; i <= new_deg; ++i) coeff[i] = temp[i];
        cur_deg = new_deg;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_cache();
        initialized = true;
    }
    if ((size_t)n > D) return (seq_t)0;
    return coeff[(size_t)n];
}
