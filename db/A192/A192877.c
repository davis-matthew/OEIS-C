/*
 * A192877  Coefficient of x in the reduction by (x^2->x+1) of the polynomial
 *          p(n,x) given in Comments.
 * https://oeis.org/A192877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 5*a(n-3) - 6*a(n-4) + 4*a(n-5);
 *            Initial values: a(0)=0, a(1)=1, a(2)=4, a(3)=14, a(4)=47
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache and bookkeeping */
    static seq_t *cache      = NULL;   /* array holding computed a(i) */
    static size_t cache_cap  = 0;      /* allocated capacity of cache   */
    static idx_t  computed   = 0;      /* largest n for which a(n) is known */

    /* Initialise on first call */
    if (cache == NULL) {
        size_t init_cap = 5;
        if ((size_t)n + 1 > init_cap) init_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)47;
        cache_cap = init_cap;
        computed   = 4;
    }

    /* Grow the cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence */
    if (computed < (idx_t)n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* s = 2*a(i-1) + 6*a(i-2) + 4*a(i-5)   (always ≥ 0) */
            seq_t s = (seq_t)2 * cache[i - 1];
            s += (seq_t)6 * cache[i - 2];
            s += (seq_t)4 * cache[i - 5];

            /* t = 5*a(i-3) + 6*a(i-4)               (always ≥ 0) */
            seq_t t = (seq_t)5 * cache[i - 3];
            t += (seq_t)6 * cache[i - 4];

            cache[i] = s - t;   /* safe: s ≥ t for all n ≥ 5 */
        }
        computed = n;
    }

    return cache[n];
}
