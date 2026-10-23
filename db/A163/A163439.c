/*
 * A163439  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1)+12*a(n-2)+12*a(n-3)+12*a(n-4)-78*a(n-5) for n ≥
 *            6 Initial values: a(0)=1, a(1)=14, a(2)=182, a(3)=2366,
 *            a(4)=30758, a(5)=399763
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t capacity = 0; /* total slots allocated */
    static idx_t filled   = 0; /* number of values actually stored */

    /* ---------- initialisation ---------- */
    if (filled == 0) {
        /* we need at least 6 slots (0..5) */
        capacity = (n + 1 >= 6) ? n + 1 : 6;
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!cache) exit(1);           /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)182;
        cache[3] = (seq_t)2366;
        cache[4] = (seq_t)30758;
        cache[5] = (seq_t)399763;
        filled = 6;
    }
    /* ---------- grow cache if needed ---------- */
    else if (n + 1 > capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap < 6) new_cap = 6;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        capacity = new_cap;
    }

    /* ---------- answer quick if already known ---------- */
    if (n < filled)
        return cache[n];

    /* ---------- compute missing terms up to n ---------- */
    for (idx_t k = filled; k <= n; ++k) {
        if (k < 6) {
            /* already initialised (k = 0..5) */
            continue;
        }
        /* recurrence for k ≥ 6 */
        seq_t val =
            (seq_t)12 * cache[k-1] +
            (seq_t)12 * cache[k-2] +
            (seq_t)12 * cache[k-3] +
            (seq_t)12 * cache[k-4] -
            (seq_t)78 * cache[k-5];
        cache[k] = val;
    }
    filled = n + 1;
    return cache[n];
}
