/*
 * A011850  a(n) = floor(binomial(n,4)/4).
 * https://oeis.org/A011850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( binomial(n,4) / 4 )
 * Range:     u8 n <= 14, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = ((n*(n-1)*(n-2)*(n-3)) / 96) for n ≥ 4, else 0 */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    seq_t nn = (seq_t)n;
    return (nn * (nn - 1) * (nn - 2) * (nn - 3)) / 96;
}
