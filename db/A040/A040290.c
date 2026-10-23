/*
 * A040290  Continued fraction for sqrt(308).
 * https://oeis.org/A040290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 17; for n > 0, a(n) repeats the pattern (1,1,4,1,1,34)
 *            every 6 terms
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 17;
    static const seq_t pat[6] = {1, 1, 4, 1, 1, 34};
    return pat[(n - 1) % 6];
}
