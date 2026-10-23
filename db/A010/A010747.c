/*
 * A010747  Shifts 6 places left under inverse binomial transform.
 * https://oeis.org/A010747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 19, i32 n <= 28, i64 n <= 44, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A010747: Shifts 6 places left under inverse binomial transform.
 * a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 16, a(5) = 32
 * a(n) = Sum_{j=0..n-6} a(n-6-j) * binomial(n-6, j) * (-1)^j   for n >= 6
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    /* static cache: computed values of the sequence up to the highest
       index requested so far.  The array is grown on demand. */
    static seq_t *cache = NULL;      /* array of a(i) */
    static size_t cache_size = 0;    /* allocated length */
    static idx_t computed_upto = 0;  /* largest index computed */
    static int initialized = 0;      /* flag */

    /* initialisation: allocate cache and set a(0)=1 */
    if (!initialized) {
        computed_upto = 0;
        cache_size = (size_t)(7);  /* small but enough for first term */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        initialized = 1;
    }

    /* grow cache if needed */
    if (n >= (idx_t)cache_size) {
        size_t new_size = (size_t)(n + 1);
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure: return 0 */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute any missing values up to n */
    if (n > computed_upto) {
        /* ensure a(1)..a(5) are computed if n >=5 */
        for (idx_t k = computed_upto + 1; k <= n; ++k) {
            if (k < 6) {
                if (k == 0) {
                    cache[k] = (seq_t)1;   /* already set, but keep for safety */
                } else {
                    /* a(k) = a(k-1) * 2 for k<6 */
                    cache[k] = cache[k - 1] * (seq_t)2;
                }
            } else {
                idx_t m = k - 6;          /* n-6 in recurrence */
                seq_t sum = (seq_t)0;
                seq_t binom = (seq_t)1;   /* binomial(m,0) */
                for (idx_t j = 0; j <= m; ++j) {
                    seq_t term = cache[m - j] * binom;
                    if (j & 1) {
                        sum -= term;
                    } else {
                        sum += term;
                    }
                    if (j < m) {
                        /* update binomial: binom = binom * (m-j) / (j+1) */
                        binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);
                    }
                }
                cache[k] = sum;
            }
        }
        computed_upto = n;
    }

    return cache[n];
}
