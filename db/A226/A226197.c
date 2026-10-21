/*
 * A226197  Numbers of vectors with 2*n integers such that each element is
 *          either 1 or -1, and their sum > n.
 * https://oeis.org/A226197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1+floor(n/2)}^n binomial(2*n, n-k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This can be rewritten as a(n) = sum_{j=0}^{floor((n-1)/2)} binomial(2*n, j) */

seq_t a(idx_t n)
{
    /* n >= 1 according to OEIS offset 1 */
    idx_t jmax = (n - 1) / 2;          /* upper bound for j */
    seq_t coeff = 1;                  /* coeff = C(2n, 0) */
    seq_t sum   = 1;                  /* accumulate the sum */
    seq_t two_n = (seq_t)n * ((seq_t)2); /* 2*n as seq_t for multiplication */

    for (idx_t j = 1; j <= jmax; ++j) {
        /* coeff = coeff * (2n - (j - 1)) / j  */
        coeff = coeff * ((seq_t)(two_n - (j - 1))) / ((seq_t)j);
        sum += coeff;
    }

    return sum;
}
