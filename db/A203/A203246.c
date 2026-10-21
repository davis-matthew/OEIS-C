/*
 * A203246  Second elementary symmetric function of the first n terms of
 *          (1,1,2,2,3,3,4,4,...).
 * https://oeis.org/A203246
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^4 + 16*n^3 + 6*n^2 - 16*n - 9 + 3*(-1)^n*(3-2*n^2))
 *            / 192
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Valid for n >= 2 (offset 2).  Implementation uses only +,-,*,/ operations on seq_t. */
seq_t a(idx_t n) {
    if (n < 2) return 0;          /* behaviour below the offset is irrelevant */

    /* Convert n to seq_t for arithmetic */
    seq_t n_t = (seq_t)n;
    seq_t n2   = n_t * n_t;            /* n^2 */
    seq_t n3   = n2 * n_t;             /* n^3 */
    seq_t n4   = n3 * n_t;             /* n^4 */

    /* Compute the numerator 192*a(n) as a sum of positive and negative parts */
    seq_t N = (seq_t)6 * n4   /* 6*n^4   */
            + (seq_t)16 * n3 /* 16*n^3  */
            + (seq_t)6  * n2 /* 6*n^2   */
            - (seq_t)9;      /* -9      */

    /* Subtract the linear term 16*n */
    N -= (seq_t)16 * n_t;

    /* Handle the parity term: +/- 3*(2*n^2-3) */
    seq_t alt = (seq_t)3 * ((seq_t)2 * n2 - (seq_t)3);   /* 3*(2*n^2-3)  */
    if ((n & 1) == 0)      /* n even: subtract */
        N -= alt;
    else                   /* n odd:  add  */
        N += alt;

    /* Division by 192 yields the desired value a(n) */
    return N / (seq_t)192;
}
