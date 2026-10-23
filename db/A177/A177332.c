/*
 * A177332  Partial sums of round(n^2/29).
 * https://oeis.org/A177332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*(n+1)*(2*n+1)/174)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 905, u64 n <= 905,
 *            u128 n <= 905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A177332: partial sums of round(k^2/29) */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1) * (2 * nn + 1);   /* product fits 64‑bit for tested range */
    return (prod + (seq_t)87) / (seq_t)174;       /* round by adding ½ of 174 */
}
