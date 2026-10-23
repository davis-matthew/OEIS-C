/*
 * A015778  Inverse of 1769th cyclotomic polynomial.
 * https://oeis.org/A015778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            * 1 / Φ_1769(x), the 1769th cyclotomic polynomial. * For n ≥ 0
 *            the sequence is periodic of period 1769. * Let r = n mod 1769.
 *            Then * a(n) = 1 if 0 ≤ r < 29 * 0 if 29 ≤ r < 61 * -1 if 61 ≤ r
 *            < 90 * 0 if 90 ≤ r < 1769 * * (This follows...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t table[1769];

    if (!init) {
        const idx_t N = 1769;
        const idx_t P = 29;
        const idx_t Q = 61;
        const idx_t PQ = P + Q;          /* 90 */

        for (idx_t r = 0; r < N; ++r) {
            seq_t val = 1;               /* contribution from i = 0 */
            if (r >= P)  val -= 1;       /* contribution from i = P  */
            if (r >= Q)  val -= 1;       /* contribution from i = Q  */
            if (r >= PQ) val += 1;       /* contribution from i = P+Q */
            table[r] = val;
        }
        init = true;
    }

    return table[n % 1769];
}
