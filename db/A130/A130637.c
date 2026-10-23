/*
 * A130637  a(n) = n*a(n-2) + a(n-5) for n >= 5 and with a(0)=0, a(1)=1,
 *          a(2)=0, a(3)=3, a(4)=0.
 * https://oeis.org/A130637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-2) + a(n-5) (n ≥ 5) with a(0)=0, a(1)=1, a(2)=0,
 *            a(3)=3, a(4)=0
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static memoisation table */
    static seq_t *cache = NULL;   /* holds computed a(0)..a(k) */
    static idx_t size   = 0;      /* number of elements stored */
    static idx_t capacity = 0;    /* allocated size of cache */

    /* Ensure the cache can hold n+1 entries */
    if (capacity < n + 1) {
        idx_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap < n + 1) new_cap <<= 1;
        cache = seq_realloc(cache, capacity, new_cap);
        capacity = new_cap;
    }

    /* Initialise base cases once */
    if (size < 5) {
        static const seq_t base[5] = {0, 1, 0, 3, 0};
        for (idx_t i = size; i < 5; ++i)
            cache[i] = base[i];
        size = 5;
    }

    /* If the requested term is already computed, return it */
    if (n < size) return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = (seq_t)i * cache[i - 2] + cache[i - 5];
    }
    size = n + 1;
    return cache[n];
}
