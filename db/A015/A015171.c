/*
 * A015171  Inverse of 1162nd cyclotomic polynomial.
 * https://oeis.org/A015171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if (n mod 1162) ≤ 6 and even; -1 if (n mod 1162) ≤ 6
 *            and odd; 0 otherwise
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 1162;
    idx_t m = n % period;
    if (m <= 6) {
        if (m & 1U)
            return (seq_t)-1;
        else
            return (seq_t)1;
    }
    return (seq_t)0;
}
