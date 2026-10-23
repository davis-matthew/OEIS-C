/*
 * A166466  Trisection a(n) = A000265(3n).
 * https://oeis.org/A166466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * odd_part(n) where odd_part(n) is n with all factors
 *            of 2 removed
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }
    return (seq_t)m * (seq_t)3;
}
