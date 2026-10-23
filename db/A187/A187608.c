/*
 * A187608  Number of 4-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-bishop's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 8.
   Initial values (n = 1..8):
   a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 28, a(5) = 144,
   a(6) = 340, a(7) = 675, a(8) = 1120. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_n = 0;   /* number of terms currently cached (1‑based) */

    /* initialise cache with first 8 terms */
    if (cache == NULL) {
        capacity = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) return 0;           /* in case malloc fails */
        cache[0] = 0;   /* a(1) */
        cache[1] = 0;   /* a(2) */
        cache[2] = 0;   /* a(3) */
        cache[3] = 28;  /* a(4) */
        cache[4] = 144; /* a(5) */
        cache[5] = 340; /* a(6) */
        cache[6] = 675; /* a(7) */
        cache[7] = 1120;/* a(8) */
        max_n = 8;
    }

    if (n <= max_n) {
        return cache[n - 1];
    }

    /* grow cache if needed */
    while (capacity < (size_t)n) {
        size_t new_capacity = capacity ? capacity * 2 : 8;
        if (new_capacity < (size_t)n) new_capacity = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_capacity);
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        capacity = new_capacity;
    }

    /* compute values from max_n+1 up to n using recurrence */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t val = 3 * cache[k - 2] - 3 * cache[k - 3] + cache[k - 4];
        cache[k - 1] = val;
    }
    max_n = n;
    return cache[n - 1];
}
