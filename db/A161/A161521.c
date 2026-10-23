/*
 * A161521  Number of reduced words of length n in the Weyl group A_22.
 * https://oeis.org/A161521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in the polynomial \prod_{k=1}^{22}
 *            (1-x^{k+1})/(1-x), which equals \prod_{k=1}^{22} (1 + x + … +
 *            x^k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 77, u128 n <= 253,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t coeff[254];       /* current convolution result   */
    static bool ready = false;     /* initialization flag         */

    if (!ready) {
        /* initialise to zero */
        for (size_t i = 0; i < 254; ++i) coeff[i] = (seq_t)0;
        coeff[0] = (seq_t)1;       /* start with polynomial 1 */

        /* successively convolve with 1 + x + … + x^k for k = 1 .. 22 */
        for (idx_t k = 1; k <= 22; ++k) {
            seq_t tmp[254];
            for (size_t i = 0; i < 254; ++i) tmp[i] = (seq_t)0;

            for (size_t i = 0; i < 254; ++i) {
                if (coeff[i] == (seq_t)0) continue;
                /* add coeff[i] to positions i+t, 0 <= t <= k  */
                for (idx_t t = 0; t <= k && i + t < 254; ++t) {
                    tmp[i + t] += coeff[i];
                }
            }

            for (size_t i = 0; i < 254; ++i) coeff[i] = tmp[i];
        }

        ready = true;
    }

    if (n >= 254) return (seq_t)0;
    return coeff[(size_t)n];
}
