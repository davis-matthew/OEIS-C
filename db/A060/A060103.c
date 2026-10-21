/*
 * A060103  Fourth column (m=3) of triangle A060102.
 * https://oeis.org/A060103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n*n + 20*n + 15) * binomial(n+4,4) / 15
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 81, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    /* compute (4*n*n + 20*n + 15) */
    seq_t poly = (seq_t)4 * nn * nn + (seq_t)20 * nn + (seq_t)15;

    /* compute binomial(n+4, 4) = (n+4)(n+3)(n+2)(n+1) / 24 */
    seq_t bin = (seq_t)(n + 4);
    bin *= (n + 3);
    bin *= (n + 2);
    bin *= (n + 1);
    bin /= (seq_t)24;  /* 24 fits in all seq_t sizes */

    /* combine, then divide by 15 */
    seq_t result = poly * bin;
    result /= (seq_t)15;

    return result;
}
