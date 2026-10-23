/*
 * A170500  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for 1 <= n <= 45, a(n) = 11*10^(n-1); for n >= 46,
 *            a(n) = (9*(a(n-1)+...+a(n-45)) + 11*a(n-46))/45
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence derived from the rational generating
   function of OEIS A170500. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently stored */
    static seq_t pow10_val = 1;     /* 10^(i-1) for next term i <= 45 */
    static seq_t sum_window = 0;    /* sum of last 45 terms ending at the last computed index */
    static bool sum_window_valid = false;

    size_t target_idx = (size_t)n;          /* index to compute up to */
    size_t new_size = target_idx + 1;       /* allocate space for indices 0..n */

    /* enlarge cache if needed */
    if (new_size > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();           /* allocation failure */
        cache = new_cache;
        /* [Optional] initialise new entries to 0 (realloc may contain garbage) */
        for (size_t i = cache_size; i < new_size; ++i) {
            cache[i] = (seq_t)0;
        }
        /* compute terms from old cache_size up to target_idx */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i <= 45) {
                cache[i] = (seq_t)11 * pow10_val;
                pow10_val = pow10_val * (seq_t)10;
            } else {
                if (!sum_window_valid) {
                    /* Should not happen: when reaching n >= 46 we must have
                       calculated sum_window after computing a[45] */
                    sum_window = (seq_t)0;
                    for (size_t j = 1; j <= 45; ++j) sum_window += cache[j];
                    sum_window_valid = true;
                }
                /* recurrence: a(n) = (9*sum_window + 11*cache[i-46]) / 45 */
                seq_t numerator = (seq_t)9 * sum_window + (seq_t)11 * cache[i - 46];
                seq_t next = numerator / (seq_t)45;
                cache[i] = next;
                /* update sliding window of the last 45 terms */
                sum_window += next;
                /* i >= 46 ensures i-45 >= 1 */
                sum_window -= cache[i - 45];
            }
            if (i == 45) {
                /* after computing a[45], initialise sliding window */
                sum_window = (seq_t)0;
                for (size_t j = 1; j <= 45; ++j) sum_window += cache[j];
                sum_window_valid = true;
            }
        }
        cache_size = new_size;
    }
    return cache[target_idx];
}
