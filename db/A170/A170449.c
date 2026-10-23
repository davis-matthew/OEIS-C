/*
 * A170449  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function:
   a[0] = 1
   for 1 ≤ n ≤ 44: a[n] = 2 + 6 * Σ_{k=0}^{n‑1} a[k]
   for n ≥ 45:     a[n] = 6 * Σ_{k=n‑44}^{n‑1} a[k] – 21 * a[n‑45]
   The sequence is memoised; each value once computed stays available.
   This implementation is O(n) time and O(n) memory.                                           */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* memoised terms   */
    static idx_t  size    = 0;     /* number of valid elements in cache */
    static idx_t  cap     = 0;     /* allocated capacity              */

    if (n < size) return cache[n]; /* value already computed */

    /* ---------- grow the cache if necessary ---------- */
    idx_t needed = n + 1;
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while (cap < needed) {
        cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* ---------- initialise first term if cache is empty ---------- */
    if (size == 0) {
        cache[0] = (seq_t)1;
        size = 1;
    }

    /* ---------- compute terms up to min(n,44) ---------- */
    idx_t limit = (n < 44 ? n : 44);
    for (idx_t i = size; i <= limit; ++i) {
        seq_t sum_prev = 0;
        for (idx_t j = 0; j < i; ++j) sum_prev += cache[j];
        seq_t p = (i == 0 ? (seq_t)1 : (seq_t)2);
        cache[i] = p + (seq_t)6 * sum_prev;
    }
    size = limit + 1;                /* at least entries up to 'limit' are valid */

    if (n <= 44) return cache[n];    /* all requested values now present */

    /* ---------- prepare sliding‑window sum of last 44 terms ---------- */
    seq_t sliding_sum = 0;
    for (idx_t j = 1; j <= 44; ++j) sliding_sum += cache[j];

    /* ---------- compute remaining terms with the homogeneous recurrence ---------- */
    for (idx_t i = size; i <= n; ++i) {              /* i ≥ 45 here */
        seq_t val          = (seq_t)6 * sliding_sum
                            - (seq_t)21 * cache[i - 45];
        cache[i]           = val;
        sliding_sum        = sliding_sum + val - cache[i - 44];
    }
    size = n + 1;
    return cache[n];
}
