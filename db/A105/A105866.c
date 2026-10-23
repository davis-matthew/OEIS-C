/*
 * A105866  A generalized Chebyshev transform of the Fibonacci numbers.
 * https://oeis.org/A105866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A105866: Generalized Chebyshev transform of the Fibonacci numbers.
   Recurrence: a(0)=0, a(1)=1, a(2)=1, a(3)=6, a(4)=9;
   a(n)=a(n‑1)+5*a(n‑2)−2*a(n‑3)−4*a(n‑4)  for n>=5.
   This implementation caches computed values so that repeated or
   out‑of‑order calls are O(1) after the needed terms have been
   generated. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;    /* pointer to array of seq_t */
    static idx_t  cap        = 0;       /* allocated capacity (>= cache_last+1) */
    static idx_t  last       = 4;       /* highest index that has been computed */
    static int    init       = 0;       /* flag: initial values set */

    /* Ensure the cache can hold term a(n). */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 5 : cap;
        while (new_cap <= n) new_cap <<= 1;   /* grow geometrically */

        /* realloc() in C++ requires a cast. */
#ifdef __cplusplus
        seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) return 0;  /* allocation failed: return zero as sentinel */

        cache = tmp;
        cap   = new_cap;
    }

    /* On first use, set the initial five terms. */
    if (!init) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)9;
        init = 1;
    }

    /* If n is already computed, return it. */
    if (n <= last) return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 5*a(i-2) - 2*a(i-3) - 4*a(i-4) */
        seq_t term = cache[i - 1];
        term += 5 * cache[i - 2];
        term -= 2 * cache[i - 3];
        term -= 4 * cache[i - 4];
        cache[i] = term;
    }
    last = n;            /* remember highest computed index */
    return cache[n];
}
