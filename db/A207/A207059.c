/*
 * A207059  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x+289)^2 = y^2.
 * https://oeis.org/A207059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-5) - a(n-10) + 578 a(1) = 119, a(2) = 231, a(3) =
 *            300, a(4) = 476, a(5) = 867, a(6) = 1496, a(7) = 2120, a(8) =
 *            2511, a(9) = 3519, a(10) = 5780 (definition from OEIS A207059)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 48, u64 n <= 110, u128 n <= 236,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    119, 231, 300, 476, 867,
    1496, 2120, 2511, 3519, 5780
};

seq_t a(idx_t n)
{
    /* Handle the offset range directly. */
    if (n <= 10) return init_vals[n - 1];

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t computed = 0;      /* largest index computed (>=10) */

    /* Initialise cache on first use. */
    if (computed == 0) {
        computed = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * (computed + 1));
        if (!cache) abort();
        for (idx_t i = 1; i <= computed; ++i)
            cache[i] = init_vals[i - 1];
    }

    /* Grow and compute up to n if required. */
    if (n > computed) {
        idx_t new_size = n + 1; /* index 0 unused */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) abort();
        cache = new_cache;

        for (idx_t i = computed + 1; i <= n; ++i) {
            /* Recurrence: a(i) = 6*a(i-5) - a(i-10) + 578 */
            seq_t term = cache[i - 5] * (seq_t)6
                       - cache[i - 10]
                       + (seq_t)578;
            cache[i] = term;
        }
        computed = n;
    }

    return cache[n];
}
