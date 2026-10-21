/*
 * A086790  a(n) = floor((1+n+2*n^2+4*n^3)/(1+2*n+n^2)).
 * https://oeis.org/A086790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((1 + (n-1) + 2·(n-1)² + 4·(n-1)³) / (1 + 2·(n-1) +
 *            (n-1)²)) Offset: 1 (so a(1) corresponds to m = 0)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the 1‑based index to m = n-1 */
    seq_t m = (seq_t)(n - 1);
    /* Compute numerator: 1 + m + 2·m² + 4·m³  */
    seq_t num = (seq_t)1 + m
            + (seq_t)2 * m * m
            + (seq_t)4 * m * m * m;
    /* Compute denominator: 1 + 2·m + m²  */
    seq_t den = (seq_t)1 + (seq_t)2 * m + m * m;
    /* Integer division truncates toward zero = floor for positive values */
    return num / den;
}
