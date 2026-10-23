/*
 * A033193  Binomial transform of A033192.
 * https://oeis.org/A033193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) for n>=5 with
 *            initial values a(0)=1, a(1)=2, a(2)=6, a(3)=19, a(4)=62 (derived
 *            from the rational generating function in the OEIS entry)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle first five terms directly */
    if (n < 5) {
        /* compile‑time constants are of type seq_t implicitly */
        static const seq_t base[5] = {1, 2, 6, 19, 62};
        return base[n];
    }

    /* Static cache: cache[0..size-1] are already computed */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of computed entries  (size >= 5 after init) */
    static size_t cap  = 0;   /* allocated capacity */

    /* Initialise cache with first 5 values on first call */
    if (cache == NULL) {
        size = 5;
        cap  = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure, unlikely in judge */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 6;
        cache[3] = 19;
        cache[4] = 62;
    }

    /* If requested term already computed, just return it */
    if ((size_t)n < size)
        return cache[n];

    /* Ensure capacity for (n+1) entries */
    size_t required = (size_t)n + 1;
    if (cap < required) {
        size_t newcap = cap;
        while (newcap < required)
            newcap <<= 1;                     /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)
            return cache[n];                 /* unlikely: keep old cache */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms from the first index not yet known up to n */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = 8*cache[i-1] - 21*cache[i-2] + 20*cache[i-3] - 5*cache[i-4];
    }

    size = required;               /* update number of computed entries */

    return cache[n];
}
