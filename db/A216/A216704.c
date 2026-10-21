/*
 * A216704  a(n) = Product_{k=1..n} (64 - 8/k).
 * https://oeis.org/A216704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} (64 - 8/k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        prod *= (seq_t)(64 * k - 8);
        prod /= (seq_t)k;
    }
    return prod;
}
