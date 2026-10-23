/*
 * A162762  Minimal number of floors an elevator must move to transport n
 *          passengers initially waiting at floors i = 1, ..., n to their
 *          destinations, floor n+1-i (= n, ..., 1), if the elevator can
 *          transport at most C = 2 persons at a time and starts at floor 1,
 *          and no one may get off the elevator before reac...
 * https://oeis.org/A162762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*n-2 for n <= 5 (with a(1)=0, a(2)=2, …, a(5)=8); a(n) =
 *            a(n-4) + 2*n for n >= 6
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* Guard against n = 0 (not part of the sequence) */
    if (n == 0) return 0;

    /* Initialisation of cache */
    if (!cache) {
        cap = 6; /* indices 0..5 inclusive */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache[0] = 0;
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = (seq_t)(2 * (seq_t)i - 2);
    }

    /* Grow cache if needed and compute new values */
    if (n >= cap) {
        idx_t old_cap = cap;
        cap = n + 1;
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        for (idx_t i = old_cap; i <= n; ++i) {
            if (i <= 5)
                cache[i] = (seq_t)(2 * (seq_t)i - 2);
            else
                cache[i] = (seq_t)(2 * (seq_t)i) + cache[i - 4];
        }
    }
    return cache[n];
}
