/*
 * A085350  Binomial transform of poly-Bernoulli numbers A027649.
 * https://oeis.org/A085350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*4^n - 3^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p4 = 1, p3 = 1;
    for (idx_t i = 0; i < n; ++i) {
        p4 *= 4;
        p3 *= 3;
    }
    return 2 * p4 - p3;
}
