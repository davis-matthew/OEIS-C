/*
 * A036915  a(n) = Sum_{k=0..n} C(2*n-2*k,n-k)^2 * C(2*n,k)^2.
 * https://oeis.org/A036915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(2*n-2*k,n-k)^2 * C(2*n,k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* external linkage required */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* allocate temporary arrays */
    size_t len = (size_t)n + 1;
    seq_t *c_sqr = (seq_t *)malloc(len * sizeof(seq_t));      /* (C(2*j,j))^2 */
    seq_t *b     = (seq_t *)malloc(len * sizeof(seq_t));      /* C(2*n,k)      */
    if (!c_sqr || !b) {
        free(c_sqr);
        free(b);
        return (seq_t)0;   /* failed allocation – caller will guard against this */
    }

    /* --------- central binomials C(2*j,j) for j = 0..n --------- */
    seq_t c = (seq_t)1;      /* C(0,0) */
    c_sqr[0] = (seq_t)1;     /* (C(0,0))^2 = 1 */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t j_s = (seq_t)j;
        seq_t numerator = ((seq_t)4 * j_s) - (seq_t)2;
        c = (c * numerator) / j_s;          /* C(2*j,j) */
        c_sqr[(size_t)j] = c * c;           /* square it */
    }

    /* --------- binomial row C(2*n,k) for k = 0..n --------- */
    seq_t b_val = (seq_t)1;    /* C(2*n,0) */
    b[0] = (seq_t)1;
    seq_t n_s = (seq_t)n;
    for (idx_t k = 0; k < n; ++k) {
        seq_t k_s = (seq_t)k;
        seq_t numerator = b_val * (n_s + n_s - k_s);   /* (2*n - k) */
        seq_t denominator = k_s + (seq_t)1;            /* (k+1)   */
        b_val = numerator / denominator;               /* C(2*n,k+1) */
        b[(size_t)(k + 1)] = b_val;
    }

    /* --------- compute the sum --------- */
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t bj = b[(size_t)(n - j)];              /* C(2*n,n-j) */
        seq_t term = c_sqr[(size_t)j] * (bj * bj);  /* combine squares */
        sum += term;
    }

    /* free temporary storage */
    free(c_sqr);
    free(b);

    return sum;
}
