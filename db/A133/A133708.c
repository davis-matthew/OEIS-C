/*
 * A133708  First differences of A047835.
 * https://oeis.org/A133708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=3}^{12} c_m * binomial(n - m + 15, 15), where *
 *            the coefficients c_m are derived from the generating function *
 *            x^3*(x+1)*(x^8+52*x^7+658*x^6+2890*x^5+4810*x^4+2890*x^3+658*x^2+52*x+1)
 *            * gives c = [1, 53, 710, 3548, 7700, 7700, 3548, 710, 53, 1] for
 *            m = 3..12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 62, u128 n <= 1003,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_calc(idx_t N, idx_t k)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (N - i + 1) / i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 0 by definition */
    if (n < 3) return 0;

    /* Coefficients c_m for m = 3..12 as described */
    const seq_t c[10] = {1, 53, 710, 3548, 7700, 7700, 3548, 710, 53, 1};

    seq_t res = 0;
    for (idx_t i = 0; i < 10; ++i) {
        idx_t m = 3 + i;
        idx_t N = n - m + 15;  /* N = (n - m) + 15 */
        if (N >= 15) {
            seq_t bin = binom_calc(N, 15);
            res += c[i] * bin;
        } else {
            /* For N < 15 the binomial coefficient is zero; skip */
            continue;
        }
    }
    return res;
}
