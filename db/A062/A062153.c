/*
 * A062153  a(n) = floor(log_3(n)).
 * https://oeis.org/A062153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊log_3(n)⌋
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t m = n;                 /* work on the index directly (uint64_t) */
    while (m >= 3) {
        m /= 3;
        ++count;
    }
    return count;
}
