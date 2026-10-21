/*
 * A054325  Seventh column of Lanczos triangle A053125 (decreasing powers).
 * https://oeis.org/A054325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * binomial(2*n+7, 6)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute binomial(2*n+7, 6) */
    idx_t m = 2 * n + 7;            /* m = 2n+7 */
    seq_t numer[6];
    for (idx_t i = 0; i < 6; ++i)
        numer[i] = (seq_t)(m - i);  /* m, m-1, ..., m-5 */

    /* divide by 720 = 2^4 * 3^2 * 5 */
    const int factors[7] = {2, 2, 2, 2, 3, 3, 5};
    for (int f = 0; f < 7; ++f) {
        for (idx_t j = 0; j < 6; ++j) {
            /* find a factor that is divisible by this prime factor */
            if (numer[j] % (seq_t)factors[f] == 0) {
                numer[j] /= (seq_t)factors[f];
                break;
            }
        }
    }

    seq_t binom = 1;
    for (idx_t i = 0; i < 6; ++i)
        binom *= numer[i];

    seq_t result = binom;

    /* multiply by 4^n */
    for (idx_t i = 0; i < n; ++i)
        result *= (seq_t)4;

    return result;
}
