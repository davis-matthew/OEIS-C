/*
 * A179133  Denominators of A178381(4*n+3)/A178381(4*n+2).
 * https://oeis.org/A179133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 18*a(n-3)-a(n-6) for n>=6,
   a(0)=2, a(1)=4, a(2)=5, a(3)=26, a(4)=68, a(5)=89 */

#include <stdlib.h>

/* Computes term a(n) of OEIS A179133. */
seq_t a(idx_t n)
{
    /* Static cache holds values 0..cache_len-1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored */
    static idx_t cache_cap = 0;   /* array capacity */

    /* Initial eight terms defined by the sequence */
    static const seq_t init[6] = {
        (seq_t)2, (seq_t)4, (seq_t)5,
        (seq_t)26, (seq_t)68, (seq_t)89
    };

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 64)
            new_cap = 64;
        seq_t *tmp = (cache == NULL)
            ? (seq_t *)malloc(new_cap * sizeof(seq_t))
            : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                          /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 6)
                cache[i] = init[i];
            else
                cache[i] = ((seq_t)18 * cache[i - 3]) - cache[i - 6];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
