/*
 * A095266  A sequence generated from the Narayana triangle considered as a
 *          matrix, or from Pascal's triangle.
 * https://oeis.org/A095266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-8 + 25*n - 30*n^2 + 15*n^3)/2 (OEIS A095266)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula yields integer values for all n ≥ 1.  */
seq_t a(idx_t n)
{
    /* cast n to seq_t once */
    seq_t x = (seq_t)n;
    seq_t n2 = x * x;          /* n^2 */
    seq_t n3 = n2 * x;         /* n^3 */
    seq_t n4 = n3 * x;         /* n^4 */

    /* compute terms of the numerator: 15·n^4 - 30·n^3 + 25·n^2 - 8·n */
    seq_t term4 = n4 * (seq_t)15;   /* 15*n^4 */
    seq_t term3 = n3 * (seq_t)30;   /* 30*n^3 */
    seq_t term2 = n2 * (seq_t)25;   /* 25*n^2 */
    seq_t term1 = x   * (seq_t)8;   /* 8*n */

    /* assemble numerator; ordering prevents negative intermediate values
       for all n≥1. */
    seq_t num = term4 - term3 + term2 - term1;

    /* divide by 2 (numerator is always even) */
    return num / (seq_t)2;
}
