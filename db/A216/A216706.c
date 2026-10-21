/*
 * A216706  a(n) = Product_{k=1..n} (100 - 10/k).
 * https://oeis.org/A216706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (100 - 10/k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
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
        seq_t numerator = ((seq_t)100 * (seq_t)k) - (seq_t)10;
        result = result * numerator / (seq_t)k;
    }
    return result;
}
