/*
 * A176760  Numbers k such that k^2 and k^4 have the same sum of digits.
 * https://oeis.org/A176760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 14, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = the n-th integer k (1‑based, a(1)=0) such that 
 * the sum of the decimal digits of k^2 equals that of k^4.
 *
 * The function builds a memoised list of terms by testing successive
 * integers until enough terms are found.
 */
#include <stddef.h>    /* size_t */
#include <stdbool.h>   /* bool */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_sum.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* memoised cache of found terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* next integer to test for the property */
static idx_t next_candidate = 0;

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)next_candidate;

        /* compute k^2 and k^4 using seq_mul */
        seq_t sq  = seq_mul(k, k);
        seq_t sq4 = seq_mul(sq, sq);

        if (digit_sum(sq) == digit_sum(sq4)) {
            /* Ensure enough capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) return 0;  /* allocation failure – unlikely for required ranges */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
