/*
 * A187381  Number of 7-step S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 487*n^2 - 3198*n + 5104 for n > 5 Recurrence: a(n) =
 *            3*a(n-1) - 3*a(n-2) + a(n-3) for n > 8 Base values: a(1)=0,
 *            a(2)=0, a(3)=18, a(4)=324, a(5)=1418, a(6)=3448, a(7)=6581,
 *            a(8)=10688 The sequence is computed automatically for any n ≥ 1.
 *            No external data tables are used beyond the eight in...
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* small constant table for the first 8 terms */
    static const seq_t base[9] = {0, 0, 0, 18, 324, 1418, 3448, 6581, 10688};

    if (n <= 8) return base[n];

    /* static cache for already computed values */
    static seq_t *cache   = NULL;   /* 1‑based index, cache[0] unused   */
    static idx_t  size    = 0;      /* last index computed             */
    static idx_t  capacity = 0;     /* allocated array size             */

    /* enlarge cache if necessary */
    if (n > capacity) {
        idx_t new_cap = (n > 16 ? n : 16);
        seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        /* copy already computed values */
        for (idx_t i = 1; i <= capacity; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        capacity = new_cap;
    }

    /* compute missing values up to n */
    if (n > size) {
        for (idx_t i = size + 1; i <= n; ++i) {
            if (i <= 8) {
                cache[i] = base[i];
            } else {
                /* recurrence: a(i) = 3*a(i‑1) – 3*a(i‑2) + a(i‑3) */
                cache[i] = 3 * cache[i-1] - 3 * cache[i-2] + cache[i-3];
            }
        }
        size = n;
    }

    return cache[n];
}
