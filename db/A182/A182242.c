/*
 * A182242  a(0)=0, a(n) = (a(n-1) + n) AND n.
 * https://oeis.org/A182242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0)=0, a(n) = (a(n-1)+n) AND n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = prev + (seq_t)i;
        prev = sum & (seq_t)i;
    }
    return prev;
}
