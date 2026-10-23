/*
 * A015276  Gaussian binomial coefficient [ n,3 ] for q = -8.
 * https://oeis.org/A015276
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,3] for q = -8 Computed
 *            by the recurrence a(3) = 1 a(n) = a(n-1) * ((-8)^n - 1) /
 *            ((-8)^{n-3} - 1) for n >= 4 To keep arithmetic exact the
 *            numerator and denominator are reduced by their gcd before the
 *            final multiplication
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 17,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"   /* provides seq_gcd(seq_t a, seq_t b) */

seq_t a(idx_t n) {
    /* values below the offset are undefined – return 0 */
    if (n < 3) return 0;
    if (n == 3) return 1;

    seq_t result = 1;                       /* a(3) */
    seq_t pow_n = (seq_t)-512;              /* (-8)^3 */
    seq_t pow_n_minus3 = (seq_t)1;          /* (-8)^0 */

    for (idx_t i = 4; i <= n; ++i) {
        /* advance powers of -8 */
        pow_n = pow_n * (seq_t)(-8);          /* (-8)^i */
        pow_n_minus3 = pow_n_minus3 * (seq_t)(-8); /* (-8)^{i-3} */

        /* compute new factor: ((-8)^i - 1) / ((-8)^{i-3} - 1) */
        seq_t num = pow_n - (seq_t)1;
        seq_t den = pow_n_minus3 - (seq_t)1;

        /* reduce with gcd to avoid overflow */
        seq_t g = seq_gcd(result, den);
        result /= g;
        den /= g;
        g = seq_gcd(num, den);
        num /= g;
        den /= g;   /* den should now be 1 */

        result = result * num;
    }
    return result;
}
