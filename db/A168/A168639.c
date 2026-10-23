/*
 * A168639  Expansion of x*(1 + x^2 - x^3) / ( (1-x)*(1-x-x^4) ).
 * https://oeis.org/A168639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) - a(n-5) * with initial values
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=4, a(4)=5
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 67, u64 n <= 136,
 *            u128 n <= 273, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;   /* array of computed terms */
static idx_t  cap   = 0;      /* allocated capacity (bytes of elements) */
static idx_t  len   = 0;      /* number of elements actually stored */

/* enlarge cache if necessary */
static void ensure_cache(idx_t need)
{
    if (cap > need) return;
    idx_t new_cap = (cap != 0 ? cap << 1 : 16);
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;                     /* allocation failure unlikely */
    if (cache) {
        for (idx_t i = 0; i < len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cap   = new_cap;
}

/* OEIS sequence A168639 */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < len) return cache[n];               /* already computed */

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0)          cache[i] = 0;
        else if (i == 1)     cache[i] = 1;
        else if (i == 2)     cache[i] = 2;
        else if (i == 3)     cache[i] = 4;
        else if (i == 4)     cache[i] = 5;
        else                 cache[i] = 2 * cache[i-1]
                                  - cache[i-2]
                                  + cache[i-4]
                                  - cache[i-5];
    }
    len = n + 1;
    return cache[n];
}
