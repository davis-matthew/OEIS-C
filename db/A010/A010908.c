/*
 * A010908  Pisot sequence E(4,21), a(n) = floor(a(n-1)^2/a(n-2) + 1/2).
 * https://oeis.org/A010908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=4, a(1)=21, a(n)=6*a(n-1)-4*a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed terms          */
    static size_t cache_cap = 0;     /* capacity of the array            */
    static size_t cache_len = 0;     /* number of terms actually stored  */

    /* Case n==0 or n==1 -- base values */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)21;

    /* Initialise cache if first call */
    if (cache == NULL) {
        cache_cap = (size_t)(n > 1 ? n + 1 : 2);          /* allocate enough space for requested index        */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return 0;                     /* out of memory – caller will get zero             */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)21;
        cache_len = 2;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;                  /* out of memory                                    */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (cache_len <= n) {
        /* a(k) = 6*a(k-1) - 4*a(k-2)   (k = cache_len) */
        seq_t term1 = cache[cache_len - 1] * (seq_t)6;
        seq_t term2 = cache[cache_len - 2] * (seq_t)4;
        cache[cache_len] = term1 - term2;
        ++cache_len;
    }

    return cache[n];
}
