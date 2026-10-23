/*
 * A163067  a(n) = 14*a(n-1) - 44*a(n-2) for n > 1; a(0) = 2, a(1) = 19.
 * https://oeis.org/A163067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 44*a(n-2); a(0)=2, a(1)=19
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Return small fixed cases directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)19;

    /* Static cache to remember previously computed values. */
    static seq_t *cache = NULL;        /* pointer to array of seq_t */
    static size_t cache_cap = 0;       /* allocated size of cache */
    static idx_t computed_max = 1;     /* highest index computed so far */

    /* Allocate or grow cache as needed. */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0 ? 2 : cache_cap * 2);
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
        /* Initialise base values if not already present. */
        if (cache_cap >= 2) {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)19;
        }
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)14 * cache[i - 1] - (seq_t)44 * cache[i - 2];
    }
    computed_max = n;

    return cache[n];
}
