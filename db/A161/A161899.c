/*
 * A161899  Number of reduced words of length n in the Weyl group B_21.
 * https://oeis.org/A161899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{21} (1+x+…+x^(2k−1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 75, u128 n <= 441,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t coef[442];
static seq_t newcoef[442];
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise coefficient array to zero */
        for (int i = 0; i < 442; ++i) coef[i] = 0;
        coef[0] = 1;
        int curr_deg = 0;
        for (idx_t k = 1; k <= 21; ++k) {
            int step = 2 * (int)k - 1;                 /* factor length minus 1 */
            /* clear newcoef up to the new degree */
            for (int i = 0; i <= curr_deg + step; ++i) newcoef[i] = 0;
            /* convolution with 1 + x + … + x^step */
            for (int i = 0; i <= curr_deg; ++i) {
                seq_t val = coef[i];
                for (int j = 0; j <= step; ++j) {
                    newcoef[i + j] += val;
                }
            }
            curr_deg += step;
            /* copy back to the main coefficient array */
            for (int i = 0; i <= curr_deg; ++i) coef[i] = newcoef[i];
        }
        initialized = 1;
    }
    if (n > 441) return 0;
    return coef[(int)n];
}
