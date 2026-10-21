/*
 * A228936  Expansion of (1 + 3*x - 3*x^3 - x^4)/(1 + 2*x^2 + x^4).
 * https://oeis.org/A228936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, cofr, easy
 * Formula:   a(0) = 1; * For n > 0: * if n is even (n = 2k): a(n) = 2 *
 *            (-1)^k * if n is odd (n = 2k+1): a(n) = 3 * (-1)^k * (2k + 1)
 * Range:     i8 n <= 42, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Determine parity and k = floor(n/2) */
    if ((n & 1) == 0) {                /* even n = 2k */
        idx_t k = n >> 1;
        seq_t sign = ((k & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        return (seq_t)2 * sign;
    } else {                           /* odd n = 2k+1 */
        idx_t k = n >> 1;              /* floor(n/2) */
        seq_t sign = ((k & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        seq_t term = (seq_t)(2 * k + 1);
        return (seq_t)3 * sign * term;
    }
}
