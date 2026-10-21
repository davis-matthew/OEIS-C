/*
 * A074724  Highest power of 3 dividing F(4n) where F(k) is the k-th Fibonacci
 *          number.
 * https://oeis.org/A074724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(v3(n)+1) (v3(n) = number of times 3 divides n)
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t v = 0;
    while (m % 3 == 0)
    {
        m /= 3;
        v++;
    }
    seq_t result = 1;
    for (idx_t i = 0; i < v + 1; ++i)
        result *= (seq_t)3;
    return result;
}
