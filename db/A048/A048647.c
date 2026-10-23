/*
 * A048647  Write n in base 4, then replace each digit '1' with '3' and vice
 *          versa and convert back to decimal.
 * https://oeis.org/A048647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base, look
 * Formula:   a(n) = if n = 0 then 0 else 4*a(floor(n/4)) + (n%4==0?0:4-(n%4))
 * Range:     u8 n <= 255, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t q = n / 4;
    idx_t m = n % 4;
    seq_t sub = a(q);
    seq_t rem = (m == 0) ? 0 : (4 - m);
    return 4 * sub + rem;
}
