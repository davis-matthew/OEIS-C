/*
 * A168570  Exponent of 3 in 2^n - 1.
 * https://oeis.org/A168570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for odd n; for even n, a(n) = 1 + v₃(n/2), where v₃(m)
 *            is the exponent of 3 dividing m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1)   /* n is odd */
        return (seq_t)0;

    idx_t m = n >> 1;   /* m = n/2 */
    seq_t v = 0;

    while (m % 3 == 0) {
        ++v;
        m /= 3;
    }

    return v + 1;
}
