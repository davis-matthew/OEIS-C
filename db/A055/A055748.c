/*
 * A055748  A chaotic cousin of the Hofstadter-Conway sequence A004001.
 * https://oeis.org/A055748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(1) = 1; a(2) = 1; a(n) = a(a(n‑1)) + a(n - a(n‑2) - 1) for n ≥
 *            3
 * Range:     u8 n <= 505, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoisation cache */
static seq_t *seq_cache = NULL;   /* 1‑based index: seq_cache[1] holds a(1) */
static idx_t  cache_len = 0;      /* current allocated length (>=1 when set) */

seq_t a(idx_t n)
{
    /* n ≥ 1 by contract; protect against zero just in case */
    if (n == 0) {
        return (seq_t)0;
    }

    /* grow cache if necessary */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        seq_cache = tmp;

        /* initialise new entries from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1 || i == 2) {
                seq_cache[i] = (seq_t)1;
            } else {
                /* indices used in the recurrence are guaranteed to be < i */
                idx_t idx1 = (idx_t)seq_cache[i - 1];                 /* a(i-1) */
                idx_t idx2 = i - (idx_t)seq_cache[i - 2] - 1;          /* i - a(i-2) - 1 */
                seq_cache[i] = seq_cache[idx1] + seq_cache[idx2];
            }
        }
        cache_len = n;
    }
    return seq_cache[n];
}
