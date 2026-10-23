/*
 * A138189  Sequence built on pattern (1,-even,-even) beginning 1,-2,-2.
 * https://oeis.org/A138189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n % 3 == 0; otherwise a(n) = -2 * (floor(n / 3) + 1)
 * Range:     i8 n <= 192, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = n % 3;
    if (rem == 0) {
        return (seq_t)1;
    } else {
        /* Convert index to seq_t before multiplication to avoid overflow */
        seq_t val = (seq_t)(n / 3) + 1;
        return -(val * 2);
    }
}
