/*
 * A145844  Number of permutations of length 2n which are invariant under the
 *          reverse-complement map and have no decreasing subsequences of
 *          length 5.
 * https://oeis.org/A145844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=0}^{n} Catalan(j) * Catalan(n-j) * C(n,j)^2 *
 *            where Catalan(k) = (2k)!/(k!(k+1)!) and C(n,j) = n!/(j!(n-j)!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Computes the nth term of OEIS sequence A145844. */
seq_t a(idx_t n)
{
    /* If n is zero, return 1 instantly. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array for Catalan numbers 0..n */
    idx_t size = n + 1;
    seq_t *cat = (seq_t *)malloc(size * sizeof(seq_t));
    if (!cat) {
        /* Out‑of‑memory: return 0 as a sentinel. */
        return (seq_t)0;
    }

    /* Compute Catalan numbers iteratively:
     * Catalan(0) = 1
     * Catalan(k+1) = Catalan(k) * 2*(2k+1) / (k+2)
     */
    cat[0] = (seq_t)1;
    for (idx_t k = 0; k < n; ++k) {
        /* numerator = cat[k] * 2 * (2*k + 1) */
        seq_t numerator = cat[k] * ((seq_t)2 * (seq_t)(2 * k + 1));
        cat[k + 1] = numerator / (seq_t)(k + 2);  /* exact division */
    }

    /* Sum over j = 0..n */
    seq_t result = (seq_t)0;
    seq_t binom   = (seq_t)1;   /* C(n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = cat[j] * cat[n - j];
        term = term * binom;
        term = term * binom;
        result += term;

        /* Update binom to C(n, j+1) for the next iteration */
        if (j < n) {
            /* C(n, j+1) = C(n, j) * (n - j) / (j + 1) */
            binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);
        }
    }

    free(cat);
    return result;
}
