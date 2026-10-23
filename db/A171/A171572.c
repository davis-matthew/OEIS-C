/*
 * A171572  a(n) = 0+1+2+...+n in lunar arithmetic in base 5 written in base
 *          10.
 * https://oeis.org/A171572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = \sum_{k>=0} min(4, floor(n / 5^k)) * 5^k
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t m = (seq_t)n;
    seq_t pow5 = 1;
    while (m > 0)
    {
        seq_t digit = (m > 4) ? (seq_t)4 : m;
        result += digit * pow5;
        m /= 5;
        pow5 *= 5;
    }
    return result;
}
