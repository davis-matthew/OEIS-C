/*
 * A068764  Generalized Catalan numbers 2*x*A(x)^2 -A(x) +1 -x =0.
 * https://oeis.org/A068764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0)=1, a(1)=1, a(n) = 2*sum_{k=0}^{n-1} a(k)*a(n-1-k)
 *            for n≥2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (generalized Catalan numbers, sequence A068764) */

#include <stdlib.h>

/* ------------  Pagination cache  ------------ */
static seq_t *cache = NULL;   /* dynamic array of computed terms   */
static idx_t  cache_size = 0; /* current number of stored terms    */

seq_t a(idx_t n)
{
    /* grow cache to at least n+1 entries */
    if ((idx_t)n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure – unlikely */
        cache = tmp;

        /* compute missing terms */
        for (idx_t j = cache_size; j < new_size; ++j) {
            if (j == 0) {
                cache[0] = (seq_t)1;
            } else if (j == 1) {
                cache[1] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < j; ++k) {
                    /* a(k)*a(j-1-k)*2 */
                    sum += cache[k] * cache[j-1-k] * (seq_t)2;
                }
                cache[j] = sum;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
