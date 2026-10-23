/*
 * A145272  First differences of successive iterated triangular numbers with
 *          seed 2.
 * https://oeis.org/A145272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = T(x(n‑1)) – x(n‑1) where x(0)=2, x(k)=T(x(k‑1)), and
 *            T(x)=x*(x+1)/2. The sequence starts with a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* Stores a(1) … a(N). */
    static size_t cap = 0;       /* Allocated capacity. */
    static idx_t max_n = 0;      /* Highest n computed so far. */
    static seq_t last_x = 0;     /* x(max_n), where x(0)=2. */

    /* Early exit for out‑of‑bounds index (offset starts at 1). */
    if (n == 0) return (seq_t)0;

    /* Initialisation on first call. */
    if (cache == NULL) {
        cap = 0;
        last_x = (seq_t)2;   /* x(0)=2. */
        max_n = 0;
    }

    /* Expand cache if necessary. */
    if ((size_t)n + 1 > cap) {
        ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));
    }

    /* Compute new terms up to requested n, caching results. */
    while (max_n < n) {
        seq_t new_x = last_x * (last_x + (seq_t)1) / (seq_t)2;  /* T(last_x) */
        seq_t diff = new_x - last_x;                           /* a(max_n+1) */
        max_n++;
        cache[max_n] = diff;
        last_x = new_x;
    }

    return cache[n];
}
