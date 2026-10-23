/*
 * A138188  Expansion of (1 - 2*x - 2*x^2 - x^3)/(1 + x + x^2 - x^3 - x^4 -
 *          x^5).
 * https://oeis.org/A138188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*floor(n/3)+1 if n mod 3 = 0, a(n) = -(2*floor(n/3)+3)
 *            if n mod 3 = 1, a(n) = 0 if n mod 3 = 2
 * Range:     i8 n <= 189, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n % 3;          /* n mod 3 */
    idx_t k = n / 3;          /* floor(n/3) */

    seq_t result;
    if (m == 0) {
        result = (seq_t)(2 * k + 1);
    } else if (m == 1) {
        result = -( (seq_t)(2 * k + 3) );
    } else {
        result = 0;
    }
    return result;
}
