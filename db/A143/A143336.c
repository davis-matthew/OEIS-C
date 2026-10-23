/*
 * A143336  Expansion of K(k) * (2 * E(k) - K(k)) / (Pi/2)^2 in powers of q
 *          where E(k), K(k) are complete elliptic integrals and q = exp(-Pi *
 *          K(k') / K(k)).
 * https://oeis.org/A143336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -8 * Σ_{d|n} d * (-1)^{d * (n/d - 1)}
 * Range:     i8 n <= 11, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* offset 0 */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;
        idx_t j = n / i;          /* complementary divisor */

        /* contribution of divisor i */
        seq_t sign_i = ((i & 1) && ((j - 1) & 1)) ? -1 : 1;
        sum += (seq_t)i * sign_i;

        /* contribution of divisor j (if distinct) */
        if (j != i) {
            seq_t sign_j = ((j & 1) && ((i - 1) & 1)) ? -1 : 1;
            sum += (seq_t)j * sign_j;
        }
    }
    return (seq_t)(-8) * sum;
}
