/*
 * A166390  Multiples of 13 whose reversal + 1 is also a multiple of 13.
 * https://oeis.org/A166390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that k % 13 == 0 and (rev(k)+1) %
 *            13 == 0 The function generates multiples of 13 in increasing
 *            order, testing the reversal condition and memoising the found
 *            values so that calls with arbitrary n are fast and independent
 *            of call order
 * Range:     u8 n <= 1, u16 n <= 401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_digits, seq_alloc_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* Not used, but keep safe */

    /* Memoisation structures.  These are volatile between calls. */
    static seq_t *cache      = NULL; /* array of found terms */
    static size_t cache_size = 0;     /* how many terms cached */
    static size_t cache_cap  = 0;     /* allocated length of cache */
    static uint64_t last_k    = 0;    /* last multiple index processed */

    /* Expand cache capacity when required. */
    if (cache_size < (size_t)n) {
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            seq_t *new_cache = seq_alloc_array(new_cap);
            if (cache) {
                /* move old data to new buffer */
                for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
                seq_free(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Generate further terms until we have n of them. */
        const seq_t MOD = 13;
        while (cache_size < (size_t)n) {
            /* next multiple of 13 */
            ++last_k;
            seq_t cand = ((seq_t)13) * ((seq_t)last_k);
            seq_t rev = reverse_digits(cand);
            seq_t revp1 = rev + 1;
            if ((revp1 % MOD) == 0) {
                cache[cache_size++] = cand;
            }
        }
    }
    /* cache is 0‑based; the OEIS index starts at 1. */
    return cache[n - 1];
}
