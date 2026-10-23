/*
 * A040557  Continued fraction for sqrt(582).
 * https://oeis.org/A040557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=24; for n>=1, a(n)=8 if n is odd, 48 if n is even
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 24;
    return (n & 1) ? 8 : 48;
}
