/*
 * A137261  G.f.:
 *          (5764801*x^8-5764801*x^7+28812*x^4-28812*x^3+840*x-1200)/(x-1).
 * https://oeis.org/A137261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -∑_{j=0}^{n-1} coeff_j where coeff_j are the coefficients
 *            of the numerator of the rational generating function: coeff_0 =
 *            -1200, coeff_1 = 840, coeff_3 = -28812, coeff_4 = 28812, coeff_7
 *            = -5764801, coeff_8 = 5764801; all other coeff_j = 0. For n ≥ 9,
 *            a(n) = 360
 * Range:     u8 none, u16 n <= 7, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    int64_t sum = 0;
    if (n > 0) sum += -1200;        /* coeff_0 */
    if (n > 1) sum += 840;          /* coeff_1 */
    if (n > 3) sum += -28812;       /* coeff_3 */
    if (n > 4) sum += 28812;        /* coeff_4 */
    if (n > 7) sum += -5764801;     /* coeff_7 */
    if (n > 8) sum += 5764801;      /* coeff_8 */
    return (seq_t)(-sum);
}
