/*
 * A098939  a(n) = (2^n - 1)^11 - 2.
 * https://oeis.org/A098939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2^n - 1)^11 - 2
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 3, i64 n <= 5, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;                     /* compute 2^n by repeated multiplication */
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
    }
    seq_t base = pow2 - 1;              /* (2^n - 1) */
    seq_t result = 1;                   /* accumulate base^11 */
    for (int i = 0; i < 11; ++i) {
        result *= base;
    }
    result -= 2;                        /* subtract 2 */
    return result;
}
