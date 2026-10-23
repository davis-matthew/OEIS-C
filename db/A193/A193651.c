/*
 * A193651  a(n) = ((2*n + 1)!! + 1)/2.
 * https://oeis.org/A193651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n+1)!! + 1)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        prod = prod * (seq_t)(2*k + 1);
    }
    return (prod + 1) / 2;
}
