/*
 * A015148  Inverse of 1139th cyclotomic polynomial.
 * https://oeis.org/A015148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - 1_{n mod 1139 >= 17} - 1_{n mod 1139 >= 67} + 1_{n
 *            mod 1139 >= 84}
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t P = 1139;
    idx_t r = n % P;          /* reduced index modulo period */
    seq_t val = 1;             /* base value */

    if (r >= 17) val -= 1;     /* subtract when index is in [17,66] */
    if (r >= 67) val -= 1;     /* subtract when index is in [67,83] */
    if (r >= 84) val += 1;     /* add when index is in [84,1138] */

    return val;                /* values are 1, 0, or -1 */
}
