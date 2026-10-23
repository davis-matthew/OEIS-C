/*
 * A041917  Denominators of continued fraction convergents to sqrt(480).
 * https://oeis.org/A041917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 482*a(n-4) - a(n-8) (n >= 8) * Initial values: a(0)=1,
 *            a(1)=1, a(2)=10, a(3)=11, * a(4)=472, a(5)=483, a(6)=4819,
 *            a(7)=5302
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for the first eight terms */
    static const seq_t base[8] = {
        (seq_t)1,  (seq_t)1,  (seq_t)10,  (seq_t)11,
        (seq_t)472, (seq_t)483, (seq_t)4819, (seq_t)5302
    };

    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t   capacity = 0;   /* number of allocated entries */
    static idx_t   last_computed = 0; /* largest index that is already in cache */

    /* If we only need one of the first eight terms, return it directly. */
    if (n < 8) {
        return base[n];
    }

    /* Allocate or grow the cache if necessary. */
    if (n + 1 > capacity) {
        idx_t new_capacity = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return 0 as a failure indicator */
            return (seq_t)0;
        }

        /* Copy existing data if this is an expansion. */
        if (cache) {
            for (idx_t i = 0; i < capacity; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }

        /* Ensure the first eight terms are available in the new array. */
        for (idx_t i = 0; i < 8 && i < new_capacity; ++i) {
            new_cache[i] = base[i];
        }

        cache = new_cache;
        capacity = new_capacity;
        last_computed = 7;   /* we have terms 0..7 computed */
    }

    /* Compute any terms that have not yet been evaluated. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* a(i) = 482 * a(i-4) - a(i-8) */
            seq_t prod = (seq_t)482 * cache[i - 4];
            cache[i] = prod - cache[i - 8];
        }
        last_computed = n;
    }

    return cache[n];
}
