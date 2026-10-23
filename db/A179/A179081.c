/*
 * A179081  Parity of sum of digits of n.
 * https://oeis.org/A179081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = parity of sum of decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    while (n > 0)
    {
        r ^= (seq_t)((n % 10) & 1);
        n /= 10;
    }
    return r;
}
