/*
 * A127876  Integers of the form (x^3)/6 + (x^2)/2 + x + 1.
 * https://oeis.org/A127876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^3-18*n^2+15*n-4)/2
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 826, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute n² and n³ with seq_t arithmetic */
    seq_t nn  = (seq_t)n * (seq_t)n;     /* n² */
    seq_t nnn = nn * (seq_t)n;           /* n³ */

    /* Evaluate each term */
    seq_t term1 = (seq_t)9  * nnn;       /* 9*n³ */
    seq_t term2 = (seq_t)18 * nn;        /* 18*n² */
    seq_t term3 = (seq_t)15 * (seq_t)n;  /* 15*n */

    /* Final result: (9*n³ - 18*n² + 15*n - 4) / 2 */
    seq_t res = (term1 - term2 + term3 - 4) / 2;
    return res;
}
