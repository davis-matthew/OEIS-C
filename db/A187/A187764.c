/*
 * A187764  Number of uud-avoiding rooted non-crossing trees of n+1 vertices
 *          with the root 1.
 * https://oeis.org/A187764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*k-2,k-1)*binomial(2*n,n-k) / n
 *            (a(0)=1). Implementation uses multiplicative generation of the
 *            needed binomial coefficients; no negative intermediate values
 *            occur
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External type definitions are supplied by the build.  */
seq_t a(idx_t n)
{
    /* Static caches for computed values.  */
    static seq_t *terms = NULL;          /* a(0..max_cached)          */
    static idx_t cache_size = 0;         /* size of allocated array   */
    static idx_t max_cached = 0;         /* highest index computed    */

    static seq_t *catalan = NULL;        /* Catalan numbers 0..maxN   */
    static idx_t cat_size = 0;           /* allocated size for Catalan */

    static seq_t *B = NULL;              /* B[k] = binom(2k-2,k-1)    */
    static idx_t B_size = 0;             /* allocated size for B      */

    static seq_t *bin = NULL;            /* temporary binom(2n, m)    */
    static idx_t bin_cap = 0;            /* allocated size for bin    */

    /* Handle the offset value n = 0.  */
    if (n == 0ULL) {
        return (seq_t)1;
    }

    /* Grow the term cache if necessary.  */
    if (cache_size == 0) {
        cache_size = 1;
        terms = (seq_t *)realloc(terms, cache_size * sizeof(seq_t));
        terms[0] = (seq_t)1;
        max_cached = 0;
    }

    if (n >= cache_size) {
        /* Extend the term cache to accommodate index n. */
        size_t new_cap = n + 1;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        cache_size = new_cap;

        /*--- Ensure Catalan numbers are available up to n. ---*/
        if (cat_size <= n) {
            size_t new_cat = n + 1;
            catalan = (seq_t *)realloc(catalan, new_cat * sizeof(seq_t));
            if (cat_size == 0) {
                catalan[0] = (seq_t)1;
                cat_size = 1;
            }
            for (idx_t i = cat_size; i <= n; ++i) {
                /* Catalan recurrence:
                   Cat(i) = Cat(i-1) * (4*(i-1)+2) / (i+1)   */
                seq_t num = catalan[i - 1] * (seq_t)(4 * (i - 1) + 2);
                catalan[i] = num / (i + 1);
            }
            cat_size = new_cat;
        }

        /*--- Ensure B(k) = binom(2k-2,k-1) up to n. ---*/
        if (B_size <= n) {
            size_t new_B = n + 1;
            B = (seq_t *)realloc(B, new_B * sizeof(seq_t));
            if (B_size == 0) {
                B[0] = (seq_t)0;          /* unused */
                B[1] = catalan[0] * (seq_t)1;  /* B(1) */
                B_size = 2;
            }
            for (idx_t k = B_size; k <= n; ++k) {
                /* B(k) = Cat(k-1) * k  */
                B[k] = catalan[k - 1] * (seq_t)k;
            }
            B_size = new_B;
        }

        /*--- Ensure temporary bin array capacity. ---*/
        if (bin_cap <= n) {
            size_t new_bin = n + 1;
            bin = (seq_t *)realloc(bin, new_bin * sizeof(seq_t));
            bin_cap = new_bin;
        }

        /*--- Compute a(i) for all new indices i > max_cached up to n. ---*/
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* Build binom(2*i, m) for m = 0..i.  */
            bin[0] = (seq_t)1;
            for (idx_t m = 0; m < i; ++m) {
                seq_t num = bin[m] * (seq_t)(2 * i - m);
                seq_t den = m + 1;
                bin[m + 1] = num / den;     /* exact division */
            }

            /* Sum the convolution: sum_{k=1..i} B[k] * bin[i-k] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += B[k] * bin[i - k];
            }

            /* Divide by i to get a(i).  */
            terms[i] = sum / (seq_t)i;
        }

        max_cached = n;
    }

    return terms[n];
}
