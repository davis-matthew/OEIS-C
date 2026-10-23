/*
 * A144758  Partial products of successive terms of A017197.
 * https://oeis.org/A144758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \prod_{k=1}^{n} (9*k-6), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)9 * k - (seq_t)6;
        result *= factor;
    }
    return result;
}
