/*
 * A064458  Highest power of 11 dividing n!.
 * https://oeis.org/A064458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊n/11⌋ + ⌊n/121⌋ + ⌊n/1331⌋ + … (highest exponent of 11
 *            in n!)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t m = n;
    while (m > 0) {
        m /= 11;          /* next power of 11 divisor */
        res += (seq_t)m;  /* add the floor of n / 11^k */
    }
    return res;
}
