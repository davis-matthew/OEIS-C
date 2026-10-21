/*
 * A092870  Expansion of Hypergeometric function F(1/12, 5/12; 1; 1728*x) in
 *          powers of x.
 * https://oeis.org/A092870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS A092870 a(0) = 1 a(n) * n^2 = a(n-1) * 12 * (12*n - 7) *
 *            (12*n - 11) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External linkage function as requested */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* stored coefficients */
    static size_t cap    = 0;       /* allocated size of cache       */
    static idx_t  max_n  = 0;       /* largest index currently cached */

    /* initialisation: a(0) = 1 */
    if (cache == NULL) {
        cap    = 1;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();        /* out of memory */
        cache[0] = (seq_t)1;
        max_n   = 0;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t prev = cache[k - 1];

        /* compute numerator: prev * 12 * (12*k-7) * (12*k-11) */
        seq_t num = prev * (seq_t)12;
        seq_t term1 = (seq_t)12 * k - (seq_t)7;
        seq_t term2 = (seq_t)12 * k - (seq_t)11;
        num = num * term1;
        num = num * term2;

        /* divide by k^2 */
        seq_t denom = k * k;
        cache[k] = num / denom;    /* exact division */
    }

    max_n = n;
    return cache[n];
}
