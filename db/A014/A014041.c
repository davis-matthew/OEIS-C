/*
 * A014041  Inverse of 32nd cyclotomic polynomial.
 * https://oeis.org/A014041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n % 16 == 0 and (n/16) % 2 == 0, * -1 if n % 16 == 0
 *            and (n/16) % 2 == 1, * 0 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Check divisibility by 16 */
    if ((n & 0xFULL) == 0) {
        /* (n/16) % 2 == 0? */
        if (((n >> 4) & 1U) == 0)
            return (seq_t)1;
        else
            return (seq_t)-1;
    } else {
        return (seq_t)0;
    }
}
