/*
 * A055999  a(n) = n*(n + 7)/2.
 * https://oeis.org/A055999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * (n + 7) / 2 (offset 0)
 * Range:     u8 n <= 19, u16 n <= 358, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before arithmetic to avoid mixing uint64_t with seq_t */
    seq_t sn = (seq_t)n;
    return sn * (sn + 7) / 2;
}
