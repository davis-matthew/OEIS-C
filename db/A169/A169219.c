/*
 * A169219  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = P(n) + 12*∑_{i=0}^{n-1} a(i) for n < 28, * a(n) = P(n)
 *            + 12*∑_{i=n-27}^{n-1} a(i) – 78*a(n-28) for n ≥ 28, * where
 *            P(0)=1, P(1..27)=2, P(28)=1, P(n>28)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#  define ALLOC_SEQ_ARRAY(cnt) new seq_t[(cnt)]
#  define FREE_SEQ_ARRAY(ptr) delete[] (ptr)
#else
#  define ALLOC_SEQ_ARRAY(cnt) ((seq_t *)malloc((cnt) * sizeof(seq_t)))
#  define FREE_SEQ_ARRAY(ptr) free(ptr)
#endif

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array holding a[0] .. a[max]   */
    static size_t cache_size = 0;    /* allocated capacity              */
    static idx_t  computed   = 0;    /* number of entries already computed (cache[0..computed-1]) */

    /* Expand cache if necessary */
    if (n >= computed) {
        size_t new_size = (size_t)(n + 1);
        if (new_size > cache_size) {
            size_t grow = cache_size > 0 ? cache_size * 2 : 16;
            if (grow < new_size)
                grow = new_size;
#if defined(__cplusplus)
            seq_t *tmp = ALLOC_SEQ_ARRAY(grow);
            if (!tmp) exit(EXIT_FAILURE);
            for (size_t i = 0; i < computed; ++i)
                tmp[i] = cache[i];
            FREE_SEQ_ARRAY(cache);
            cache = tmp;
#else
            seq_t *tmp = (seq_t *)realloc(cache, grow * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
#endif
            cache_size = grow;
        }

        /* Compute terms from 'computed' up to 'n' */
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i < 28) {
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j)
                    sum += cache[j];
                cache[i] = 2 + sum * (seq_t)12;
            } else {
                seq_t p = (i == 28) ? 1 : 0;
                seq_t sumPrev27 = 0;
                for (idx_t j = i - 27; j < i; ++j)
                    sumPrev27 += cache[j];
                seq_t term = sumPrev27 * (seq_t)12;
                term -= cache[i - 28] * (seq_t)78;
                cache[i] = p + term;
            }
        }
        computed = n + 1;
    }
    return cache[n];
}
