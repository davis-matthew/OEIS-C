/*
 * A203652  1/25 the number of (n+1)X5 0..4 arrays with every 2X2 subblock
 *          having equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 69*a(n-1) +2528*a(n-2) -75120*a(n-3) +59456*a(n-4)
 *            +1531904*a(n-5) -345088*a(n-6) -1646592*a(n-7) (empirical)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 7 terms of the sequence */
    static const seq_t init_val[7] = {
        (seq_t)6561ULL,
        (seq_t)519445ULL,
        (seq_t)44577561ULL,
        (seq_t)3906948333ULL,
        (seq_t)343812029649ULL,
        (seq_t)30292030417413ULL,
        (seq_t)2669260624372937ULL
    };

    if (n <= 7u) return init_val[n - 1];

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;   /* number of terms currently stored */

    /* Ensure the cache can hold up to n terms */
    size_t need = (size_t)n;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first seven entries if not already done */
    if (cache_size < 7) {
        for (size_t i = 0; i < 7; ++i) cache[i] = init_val[i];
        cache_size = 7;
    }

    /* Compute missing terms up to n */
    for (idx_t k = (idx_t)(cache_size + 1); k <= n; ++k) {
        /* All indices here are 1‑based for sequence terms:
           a(k) -> cache[k-1]   and a(k-1) -> cache[k-2]   etc.          */
        cache[k - 1] =
            ((seq_t)69   * cache[k - 2]) +
            ((seq_t)2528 * cache[k - 3]) -
            ((seq_t)75120 * cache[k - 4]) +
            ((seq_t)59456 * cache[k - 5]) +
            ((seq_t)1531904 * cache[k - 6]) -
            ((seq_t)345088 * cache[k - 7]) -
            ((seq_t)1646592 * cache[k - 8]);
    }

    if (cache_size < n) cache_size = n;   /* keep size up to the largest computed */

    return cache[n - 1];
}
