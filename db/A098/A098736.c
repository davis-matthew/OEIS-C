/*
 * A098736  a(n) = product of n and all its digits.
 * https://oeis.org/A098736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n * (product of decimal digits of n)
 * Range:     u8 n <= 25, u16 n <= 378, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t t = n;
    while (t > 0)
    {
        prod *= (seq_t)(t % 10);
        t /= 10;
    }
    return (seq_t)n * prod;
}
