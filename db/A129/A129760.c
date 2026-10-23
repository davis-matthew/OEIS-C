/*
 * A129760  Bitwise AND of binary representation of n-1 and n.
 * https://oeis.org/A129760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, hear, base
 * Formula:   a(n) = n AND (n-1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for n < 1; not used in tests */
    seq_t sn = (seq_t)n;
    return sn & (sn - 1);
}
