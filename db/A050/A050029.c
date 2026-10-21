/*
 * A050029  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A050029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 77, u32 n <= 517, u64 n <= 4470,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050029
 * Formula: a(n) = a(n-1) + a(2^ceil(log₂(n-1)) + 2 – n)  for n ≥ 4,
 * a(1) = 1, a(2) = 1, a(3) = 2.  All indices are 1‑based. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Compute ceil(log₂(x)) for x ≥ 1.  Returns 0 for x ≤ 1. */
static idx_t ceil_log2_idx(idx_t x)
{
    if (x <= 1) return 0;
    idx_t k = 0;
    idx_t v = 1;
    while (v < x) {
        v <<= 1;
        ++k;
    }
    return k;
}

/* Cache for previously computed terms.  The cache is grown on demand
 * and filled using the recurrence.  The sequence is defined for n ≥ 1. */
seq_t a(idx_t n)
{
    if (!n) return (seq_t)0;              /* undefined for n = 0 */

    static seq_t *cache = NULL;           /* array with indices 0..max_n  */
    static idx_t max_n = 0;               

    /* Initialise: indices 1,2,3 are known. */
    if (!cache) {
        cache = (seq_t*)calloc(4, sizeof(seq_t));  /* indices 0..3 */
        if (!cache) return (seq_t)0;               /* allocation failed */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        max_n  = 3;
    }

    /* Grow the cache if the requested index exceeds it. */
    if (n > max_n) {
        idx_t new_size = n + 1;                 /* up to n inclusive */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        cache = tmp;
        /* Zero any newly allocated slots. */
        memset(cache + max_n + 1, 0,
               (new_size - (max_n + 1)) * sizeof(seq_t));

        /* Compute all missing terms. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 3) continue;              /* base terms are ready */

            idx_t k      = ceil_log2_idx(i - 1);          /* ceil(log₂(i‑1)) */
            idx_t power  = ((idx_t)1) << k;               /* 2^k */
            idx_t m      = power + 2 - i;                 /* m = 2^k + 2 – i */
            cache[i] = cache[i - 1] + cache[m];
        }
        max_n = n;
    }

    return cache[n];
}
