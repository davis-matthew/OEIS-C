/*
 * A054897  a(n) = Sum_{k>0} floor(n/8^k).
 * https://oeis.org/A054897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/8⌋ + ⌊n/64⌋ + ⌊n/512⌋ + ... (a(0)=0, a(1)=0, …,
 *            a(7)=0)
 * Range:     u8 n <= 1799, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t  m   = n;
    while (m > 0)
    {
        m /= 8;          /* next power of 8 */
        sum += (seq_t)m;
    }
    return sum;
}
