/*
 * A197351  a(0)=0, a(1)=1, a(2n)=17*a(n), a(2n+1)=a(2n)+1.
 * https://oeis.org/A197351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0)=0, a(1)=1; a(2n)=17*a(n), a(2n+1)=a(2n)+1 Equivalent:
 *            a(n)=∑_{k≥0} (bit_k(n) * 17^k)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t mult   = 1;          /* 17^0 */
    idx_t temp   = n;
    while (temp != 0)
    {
        if (temp & 1)
            result += mult;
        mult *= (seq_t)17;
        temp >>= 1;
    }
    return result;
}
