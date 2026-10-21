/*
 * A052593  E.g.f. 1/(1-x-x^4).
 * https://oeis.org/A052593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6 and for n≥4 a(n) = n*a(n‑1) +
 *            n*(n‑1)*(n‑2)*(n‑3)*a(n‑4) (EGF 1/(1‑x‑x⁴))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements stored */

    /* Ensure cache holds values up to index n */
    if (cache == NULL || n >= cache_size) {
        idx_t needed = n + 1;                    /* indices 0 … n inclusive */
        seq_t *tmp;

        if (cache == NULL) {
            /* allocate new array */
            tmp = (seq_t *)malloc((size_t)needed * sizeof(seq_t));
        } else {
            /* enlarge existing array */
            tmp = (seq_t *)realloc(cache,
                                   (size_t)needed * sizeof(seq_t));
        }
        if (tmp == NULL) abort();                /* allocation failure */

        cache = tmp;

        /* Compute all values from the first uncomputed index */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i < 4) {                  /* a(i) = i * a(i-1) */
                cache[i] = ((seq_t)i) * cache[i - 1];
            } else {
                seq_t term1 = ((seq_t)i) * cache[i - 1];

                seq_t prod = cache[i - 4];
                prod *= (seq_t)i;
                prod *= (idx_t)(i - 1);
                prod *= (idx_t)(i - 2);
                prod *= (idx_t)(i - 3);

                cache[i] = term1 + prod;
            }
        }

        cache_size = needed;
    }

    return cache[n];
}
