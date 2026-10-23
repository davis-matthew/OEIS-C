/*
 * A156611  a(1) = 2, a(n+1) is smallest prime >= a(n)+2*n.
 * https://oeis.org/A156611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(n+1) = smallest prime ≥ a(n) + 2*n (n ≥ 1)
 * Range:     u8 n <= 15, u16 n <= 220, u32 n <= 221, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequencing logic relies on a generic prime test suitable for any seq_t.
   Use the bundled is_prime_big routine to handle arbitrary‑precision types
   and small integral types alike. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* 1‑indexed cache of computed terms */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static size_t cache_cap = 0;     /* allocated capacity in elements */
    static idx_t last_computed = 0;  /* highest n for which cache is valid */

    if (n == 0) return 0;                 /* undefined for n<1, harmless */

    /* Grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        /* Explicit cast so that C++ accepts the void* result of realloc */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) {                 /* a(1) = 2 */
            cache[0] = (seq_t)2;
            continue;
        }

        seq_t prev = cache[i - 2];                       /* a(i) = a(i-1) */
        seq_t inc  = (seq_t)((i - 1) * 2);               /* 2*(i‑1) */
        seq_t cand = prev + inc;                         /* lower bound */

        /* Skip even numbers > 2 */
        if (cand > (seq_t)2 && !(cand & (seq_t)1))
            cand += (seq_t)1;

        /* Find the smallest prime ≥ cand */
        while (!is_prime_big(cand))
            cand += (seq_t)2;     /* only odd increments after the first odd */

        cache[i - 1] = cand;      /* store a(i) */
    }

    last_computed = n;
    return cache[n - 1];
}
