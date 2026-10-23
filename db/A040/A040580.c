/*
 * A040580  Continued fraction for sqrt(605).
 * https://oeis.org/A040580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = 24 if n=0; otherwise a(n) follows a period of length 12:
 *            * (1, 1, 2, 11, 1, 8, 1, 11, 2, 1, 1, 48) with a(12)=48 and the
 *            pattern repeats
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;
    static const seq_t block[12] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)11,
        (seq_t)1, (seq_t)8, (seq_t)1, (seq_t)11,
        (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)48
    };
    return block[(n - 1) % 12];
}
