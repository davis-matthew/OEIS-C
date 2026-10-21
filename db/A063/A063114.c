/*
 * A063114  a(n) = n + product of the nonzero digits of n.
 * https://oeis.org/A063114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) = n + product of the nonzero digits of n
 * Range:     u8 n <= 188, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t x = n;
    while (x != 0)
    {
        unsigned digit = x % 10;
        if (digit != 0)
            prod = prod * (seq_t)digit;
        x /= 10;
    }
    return (seq_t)n + prod;
}
