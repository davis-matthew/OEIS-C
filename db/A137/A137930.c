/*
 * A137930  The sum of the principal diagonals of an n X n spiral.
 * https://oeis.org/A137930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 6*n^2 + 16*n + 9*((-1)^n - 1)) / 12
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 1563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             = sum of the numbers on both principal diagonals of an n×n spiral   */
seq_t a(idx_t n)
{
    /* compute n^2 and n^3 in seq_t, avoiding overflow for n <= 10000 */
    seq_t n2 = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t n3 = n2 * (seq_t)n;              /* n^3 */

    /* numerator of the formula, all additions are in seq_t */
    seq_t N = (seq_t)8 * n3;               /* 8*n^3   */
    N += (seq_t)6 * n2;                   /* +6*n^2   */
    N += (seq_t)16 * (seq_t)n;            /* +16*n    */
    if (n & 1)                            /* odd n: subtract 18  (9*(-1-1) = -18) */
        N -= (seq_t)18;
    /* even n: term4 contributes 0, so nothing to add */

    /* common denominator 12 */
    return N / (seq_t)12;
}
