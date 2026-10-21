/*
 * A203837  Expansion of x^2*(1-x^2)*(1-3*x^2)/(1-x-5*x^2+4*x^3+5*x^4-3*x^5).
 * https://oeis.org/A203837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+5*a(n-2)-4*a(n-3)-5*a(n-4)+3*a(n-5)+b(n), where
 *            b(2)= 1, b(4)= -4, b(6)= 3, and b(n)=0 otherwise. This
 *            recurrence follows from the generating function
 *            x^2*(1-x^2)*(1-3*x^2)/(1-x-5*x^2+4*x^3+5*x^4-3*x^5)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static size_t cache_cap = 0;   /* allocated size of cache */
static idx_t cache_max = (idx_t)-1;   /* highest index computed so far */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* initialise cache with first five terms (a(0) .. a(4)) */
    if (cache_max == (idx_t)-1) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache_max = 4;
    }

    /* enlarge cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* already computed? */
    if (n <= cache_max) return cache[n];

    /* compute values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term = cache[i-1];                 /* a(i-1) */

        /* recurrence contributions */
        term += cache[i-2] * (seq_t)5;           /* +5*a(i-2) */
        term -= cache[i-3] * (seq_t)4;           /* -4*a(i-3) */
        term -= cache[i-4] * (seq_t)5;           /* -5*a(i-4) */
        term += cache[i-5] * (seq_t)3;           /* +3*a(i-5) */

        /* add numerator term b(i) */
        if (i == (idx_t)2)     term += (seq_t)1;
        else if (i == (idx_t)4) term -= (seq_t)4;
        else if (i == (idx_t)6) term += (seq_t)3;

        cache[i] = term;
    }

    cache_max = n;
    return cache[n];
}
