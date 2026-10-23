/*
 * A167922  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥1,
   a(n) = 13*a(n‑1) – 90*a(n‑16) + 78*a(n‑17) + d(n)
   where d(1)=1, d(16)=d(17)=−1, and d(n)=0 otherwise. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   computed = 0;

    /* initialise cache with a(0)=1 */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache[0] = (seq_t)1;
        cap = 1;
        computed = 0;
    }

    /* extend cache if needed */
    if (n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* reallocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)13 * cache[i - 1];

        if (i >= 16) val -= (seq_t)90 * cache[i - 16];
        if (i >= 17) val += (seq_t)78 * cache[i - 17];

        if (i == 1)          val += (seq_t)1;
        else if (i == 16)    val -= (seq_t)1;
        else if (i == 17)    val -= (seq_t)1;

        cache[i] = val;
    }
    computed = n;

    return cache[n];
}
