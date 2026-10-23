/*
 * A147685  Squares and centered square numbers interleaved.
 * https://oeis.org/A147685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*m) = m^2, a(2*m+1) = 2*m^2 + 2*m + 1 (m = floor(n/2))
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        idx_t m = n >> 1;
        seq_t mm = (seq_t)m;
        return mm * mm;
    } else {
        idx_t m = (n - 1) >> 1;
        seq_t mm = (seq_t)m;
        return (seq_t)2 * mm * mm + (seq_t)2 * mm + (seq_t)1;
    }
}
