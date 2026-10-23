/*
 * A008805  Triangular numbers repeated.
 * https://oeis.org/A008805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n/2)+2, 2)
 * Range:     u8 n <= 43, u16 n <= 721, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    idx_t k = n >> 1;
    /* Compute (k+2 choose 2) = (k+2)*(k+1)/2 with one operand divided first */
    seq_t v1 = (seq_t)k + 2;   /* k+2 */
    seq_t v2 = (seq_t)k + 1;   /* k+1 */

    if ((v2 & (seq_t)1) == 0)          /* if (k+1) is even */
        return (v2 >> 1) * v1;        /* ((k+1)/2) * (k+2) */
    else
        return (v1 >> 1) * v2;        /* ((k+2)/2) * (k+1) */
}
