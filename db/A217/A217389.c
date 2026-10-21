/*
 * A217389  Partial sums of the ordered Bell numbers (number of preferential
 *          arrangements) A000670.
 * https://oeis.org/A217389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σ_{k=0..n} A000670(k) (partial sums of the ordered Bell
 *            numbers)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses memoisation and allocates the needed arrays with
   malloc/realloc.  All arithmetic is performed with the type seq_t, so the
   routine works for 8,16,32,64‑bit unsigned integers as well as for the
   arbitrary‑precision integer class used in the C++ build. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values of A000670 (ordered Bell numbers) and their partial sums. */
    static seq_t *t_cache = NULL;   /* t_cache[i] = A000670(i) */
    static seq_t *a_cache = NULL;   /* a_cache[i] = Σ_{k=0..i} t_cache[k] */
    static size_t t_cap = 0, a_cap = 0;
    static idx_t max_computed = 0;

    /* Initialisation on the first call. */
    if (t_cache == NULL) {
        t_cap = 1;
        a_cap = 1;
        t_cache = (seq_t *)malloc(t_cap * sizeof(seq_t));
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        /* base case t(0) = 1, so a(0) = 1 */
        t_cache[0] = (seq_t)1;
        a_cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* Grow the cached arrays if necessary.  Use doubling until capacity > n. */
    if ((size_t)n + 1 > t_cap) {
        size_t new_cap = t_cap ? t_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        t_cache = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
        t_cap = new_cap;
    }
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* Compute all missing values up to n. */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;           /* binom(m,0) */
        for (idx_t k = 0; k < m; ++k) {
            sum += binom * t_cache[k];
            /* binom = binom * (m-k) / (k+1) */
            binom = binom * ( (seq_t)(m - k) ) / ( (seq_t)(k + 1) );
        }
        t_cache[m] = sum;
        a_cache[m] = a_cache[m - 1] + sum;
    }

    max_computed = n;
    return a_cache[n];
}
