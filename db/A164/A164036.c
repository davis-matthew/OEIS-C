/*
 * A164036  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30 * Σ_{k=1..5} a(n-k) - 465 * a(n-6) + (n==6 ? 1 : 0)
 *            With initial values: a(0)=1, a(1)=32, a(2)=992, a(3)=30752,
 *            a(4)=953312, a(5)=29552672 (these are the first six terms of
 *            OEIS A164036)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure cache is large enough and initialise first six values */
    if (cache == NULL || cap <= 5) {
        size_t new_cap = (cap < 6 ? 6 : cap);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp; cap = new_cap;
        /* Initialise the first six terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)992;
        cache[3] = (seq_t)30752;
        cache[4] = (seq_t)953312;
        cache[5] = (seq_t)29552672;
    } else if (cap <= (size_t)n) {
        /* Grow cache to accommodate index n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp; cap = new_cap;
    }

    /* Return directly for the first six indices */
    if (n <= 5) return cache[n];

    /* Compute further values up to n */
    for (size_t i = 6; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        sum += cache[i-1];
        sum += cache[i-2];
        sum += cache[i-3];
        sum += cache[i-4];
        sum += cache[i-5];
        seq_t tmp = sum * (seq_t)30
                  - (seq_t)465 * cache[i-6];
        if (i == 6) tmp += (seq_t)1;  /* N(6) = 1 from the numerator */
        cache[i] = tmp;
    }

    return cache[n];
}
