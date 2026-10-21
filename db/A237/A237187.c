/*
 * A237187  Total number of possible evolutions arising from n tandem
 *          duplications of DNA.
 * https://oeis.org/A237187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (4^k - (2*k+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term. Undefined for n = 0, return 0 safely */
    if (n == 0) return 0;

    /* static memoisation for arbitrary-order calls */
    static idx_t cached = 0;                  /* highest index computed so far */
    static seq_t cached_prod = (seq_t)1;      /* a(cached) value */
    static seq_t cached_pow = (seq_t)4;       /* 4^(cached+1), ready for next k */
    static seq_t *cache = NULL;               /* array of a(1)..a(cached) */
    static size_t cache_cap = 0;              /* allocated capacity of cache */

    if (n <= cached)                          /* value already cached */
        return cache[n-1];

    /* enlarge cache array if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : n;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cached+1 up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t term = cached_pow - (seq_t)(2 * i + 1);
        cached_prod *= term;
        cache[i-1] = cached_prod;          /* store a(i) */
        cached_pow *= (seq_t)4;             /* power of 4 for next k */
    }

    cached = n;                              /* update highest cached index */
    return cache[n-1];
}
