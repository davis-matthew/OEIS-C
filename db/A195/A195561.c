/*
 * A195561  Hypotenuses of primitive Pythagorean triples in A195559 and
 *          A195560.
 * https://oeis.org/A195561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1298*a(n-5) - a(n-10) for n > 10, with initial values
 *            a(1)=1, a(2)=5, a(3)=109, a(4)=289, a(5)=1009, a(6)=1189,
 *            a(7)=6485, a(8)=141481, a(9)=375121, a(10)=1309681
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_end = 0;

    if (n <= cache_end) {
        return cache[n];
    }

    size_t new_capacity = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – return 0 as a fallback. */
        return 0;
    }
    cache = new_cache;

    static const seq_t init[11] = {
        0,          /* index 0 unused */
        1,          /* a(1) */
        5,          /* a(2) */
        109,        /* a(3) */
        289,        /* a(4) */
        1009,       /* a(5) */
        1189,       /* a(6) */
        6485,       /* a(7) */
        141481,     /* a(8) */
        375121,     /* a(9) */
        1309681     /* a(10) */
    };

    for (idx_t k = cache_end + 1; k <= n; ++k) {
        if (k <= 10) {
            cache[k] = init[k];
        } else {
            cache[k] = (seq_t)1298 * cache[k - 5] - cache[k - 10];
        }
    }
    cache_end = n;
    return cache[n];
}
