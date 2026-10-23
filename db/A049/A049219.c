/*
 * A049219  Number of horizontally convex n-ominoes in which the top row has
 *          exactly 1 square.
 * https://oeis.org/A049219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 4*a(n-3) for n >= 5 a(1) = 1, a(2)
 *            = 1, a(3) = 3, a(4) = 10
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of already computed terms (zero‑based: cache[0] = a(1)) */
static seq_t *cache    = NULL;
static size_t cache_cap = 0;   /* number of allocated slots */

/* Return the n-th term of A049219 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, safety return */

    /* Grow cache to contain at least the first n terms. */
    if (cache_cap < (size_t)n) {
        size_t needed = n;
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */

        cache = tmp;

        /* Initialise new entries. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            idx_t idx = (idx_t)(i + 1);   /* 1‑based index */

            seq_t val;
            if (idx == 1) {
                val = 1;
            } else if (idx == 2) {
                val = 1;
            } else if (idx == 3) {
                val = 3;
            } else if (idx == 4) {
                val = 10;
            } else {
                /* recurrence a(n) = 5*a(n-1) - 7*a(n-2) + 4*a(n-3) */
                seq_t a1 = cache[i   - 1];  /* a(n-1) */
                seq_t a2 = cache[i   - 2];  /* a(n-2) */
                seq_t a3 = cache[i   - 3];  /* a(n-3) */

                val = 5 * a1 - 7 * a2 + 4 * a3;
            }
            cache[i] = val;
        }
        cache_cap = new_cap;
    }

    return cache[n - 1];
}
