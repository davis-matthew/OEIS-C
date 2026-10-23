/*
 * A126498  Number of base 30 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126498: Number of base‑30 n‑digit numbers with adjacent digits
 * differing by at most 3.
 *
 * DP formulation:
 *
 *   dp_k[d] = number of length‑k sequences ending with digit d (0 ≤ d ≤ 29)
 *   dp_1[d] = 1                                  (every digit alone is valid)
 *   dp_k+1[d] = Σ_{|d'−d|≤3} dp_k[d']            (sum over digits within distance 3)
 *
 *   a(n) = Σ_{d=0}^{29} dp_n[d]  for n ≥ 1,   a(0)=1
 *
 * Implementation uses memoisation: previously computed values are cached
 * in a static array.  The function works for any build where
 * seq_t is a trivially constructible integer type (including
 * arbitrary‑precision integer classes that are trivially default‑constructible).
 * The minimal dynamic allocation needed for the cache uses malloc/realloc
 * with explicit casts to obtain a `seq_t *` pointer, which is valid under
 * both C11 and C++17.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0 .. cache_len-1] holds a(0) .. a(cache_len-1) */
    static size_t cache_len = 0;      /* number of stored entries                      */
    static seq_t dp_prev[30];        /* dp values for length (cache_len-1)            */

    /* 1. Expand cache if needed */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        if (!cache) {
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
            if (!cache) return (seq_t)0;           /* allocation failed */
        } else if (new_len > cache_len) {
            seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failed */
            cache = tmp;
        }

        /* 2. Initialise missing entries */
        while (cache_len <= n) {
            /* a(0) = 1 */
            if (cache_len == 0) {
                cache[0] = (seq_t)1;
                cache_len = 1;
                continue;
            }

            /* a(1) = 30 (all single digits are allowed) */
            if (cache_len == 1) {
                seq_t sum = 0;
                for (idx_t d = 0; d < 30; ++d) {
                    dp_prev[d] = (seq_t)1;
                    sum += (seq_t)1;
                }
                cache[1] = sum;     /* 30 */
                cache_len = 2;
                continue;
            }

            /* For n >= 2 : compute dp for current length from dp_prev */
            seq_t dp_cur[30];
            seq_t sum = 0;
            for (idx_t d = 0; d < 30; ++d) {
                seq_t s = 0;
                idx_t low  = (d > 3) ? d - 3 : 0;
                idx_t high = (d + 3 < 29) ? d + 3 : 29;
                for (idx_t d2 = low; d2 <= high; ++d2) {
                    s += dp_prev[d2];
                }
                dp_cur[d] = s;
                sum += s;
            }

            cache[cache_len++] = sum;

            /* Prepare dp_prev for the next iteration */
            for (idx_t d = 0; d < 30; ++d) {
                dp_prev[d] = dp_cur[d];
            }
        }
    }

    return cache[n];
}
