/*
 * A198864  a(0) = 1; a(n) = n + product_{i=0..n-1} a(i), n >= 1.
 * https://oeis.org/A198864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + product_{i=0}^{n-1} a(i)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    seq_t val;
    for (idx_t i = 0; i < n; ++i)
    {
        val = (seq_t)i + prod;
        prod *= val;
    }
    return (seq_t)n + prod;
}
