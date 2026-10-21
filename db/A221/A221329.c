/*
 * A221329  Number of 2 X n arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor, with no
 *          occupancy greater than 2.
 * https://oeis.org/A221329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 27*a(n-2) - a(n-3) + 8*a(n-4) - 16*a(n-5) -
 *            6*a(n-6) + 2*a(n-7) for n > 9 Initial values (offset 1): a(1)=1,
 *            a(2)=17, a(3)=119, a(4)=866, a(5)=6328, a(6)=46211, a(7)=337274,
 *            a(8)=2460918, a(9)=17953576
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence values are stored in a static cache for repeated calls.
   We use malloc/realloc to grow the cache as needed.  This works for
   both native integer types and arbitrary‑precision types that
   support the usual arithmetic operators. */

seq_t a(idx_t n)
{
    /* Basic values up to n = 9 */
    static const seq_t base_vals[10] = {0, 1, 17, 119, 866,
                                        6328, 46211, 337274,
                                        2460918, 17953576};
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache array */
    static idx_t  cache_size = 0;  /* largest index computed */

    if (n == 0) return 0;  /* unused but safe */

    /* Allocate or grow cache to hold at least 'n' */
    if (cache == NULL) {
        cache_cap = (size_t)(n > 9 ? n + 1 : 10);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 1; i <= 9; ++i) {
            cache[i] = base_vals[i];
        }
        cache_size = 9;
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 1; i <= cache_size; ++i) new_cache[i] = cache[i];
        /* Old cache is leaked but size grows only logarithmically. */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t val = cache[k-1] * (seq_t)11
                    - cache[k-2] * (seq_t)27
                    - cache[k-3]
                    + cache[k-4] * (seq_t)8
                    - cache[k-5] * (seq_t)16
                    - cache[k-6] * (seq_t)6
                    + cache[k-7] * (seq_t)2;
        cache[k] = val;
    }
    cache_size = n > cache_size ? n : cache_size;
    return cache[n];
}
