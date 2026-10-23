/*
 * A180318  Expansion of a(-q) in powers of q where a(q) is a cubic AGM
 *          function.
 * https://oeis.org/A180318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * 6 * (d1(n) - d2(n)) where d1(n) is the number of
 *            divisors of n congruent to 1 mod 3, d2(n) is the number of
 *            divisors of n congruent to 2 mod 3. Derived from the
 *            representation j^2 + j*k + k^2 = n
 * Range:     i8 n <= 8859, i16 n <= 8859, i32 n <= 8859, i64 n <= 8859,
 *            i128 n <= 8859, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt(idx_t n) {
    if (n < 2) return n;
    idx_t left = 1, right = n >> 1, res = 1;
    while (left <= right) {
        idx_t mid = (left + right) >> 1;
        if (mid <= n / mid) {      /* mid*mid <= n */
            res = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;           /* a(0) = 1 */

    idx_t nn = n;
    idx_t limit = isqrt(nn);
    idx_t diff = 0;                 /* d1 - d2 */

    for (idx_t i = 1; i <= limit; ++i) {
        if (nn % i == 0) {
            /* divisor i */
            if (i % 3 == 1) ++diff;
            else if (i % 3 == 2) --diff;

            /* complementary divisor j = n / i */
            idx_t j = nn / i;
            if (j != i) {
                if (j % 3 == 1) ++diff;
                else if (j % 3 == 2) --diff;
            }
        }
    }

    seq_t mult = (seq_t)6 * (seq_t)diff;
    if (n & 1ULL) mult = -mult;     /* multiply by (-1)^n */
    return mult;
}
