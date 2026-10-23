/*
 * A193683  Alternating row sums of Sheffer triangle A143495 (3-restricted
 *          Stirling2 numbers).
 * https://oeis.org/A193683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; a(n)=3*a(n-1)-sum_{k=0}^{n-1} C(n-1,k)*a(k) (n>=1)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 26, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t);

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of entries currently stored   */
    static size_t cache_cap  = 0;      /* allocated capacity                    */

    /* ensure cache large enough for index n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        cache = tmp;                      /* update pointer after realloc */
        cache_cap = new_cap;
    }

    /* compute missing entries up to n */
    for (size_t idx = cache_size; idx <= (size_t)n; ++idx) {
        if (idx == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(idx-1,0) */
        for (size_t k = 0; k < idx; ++k) {
            /* sum += binom * a[k] */
            sum += binom * cache[k];

            /* update binom for next k: C(idx-1,k+1) = C(idx-1,k)*(idx-1-k)/(k+1) */
            seq_t numer   = (seq_t)(idx - 1 - k);
            seq_t denom   = (seq_t)(k + 1);
            binom = (binom * numer) / denom;   /* exact division */
        }

        seq_t three = (seq_t)3;
        cache[idx] = three * cache[idx - 1] - sum;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
