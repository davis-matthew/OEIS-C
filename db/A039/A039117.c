/*
 * A039117  Numbers whose base-10 representation has the same number of 0's
 *          and 6's.
 * https://oeis.org/A039117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_zeros_and_sixes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039117: numbers whose decimal representation has equal number
   of 0 and 6 digits.  Computed by enumeration. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_zeros_and_sixes.h"

/* static cache of the found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;   /* next number to test */

/* ensure that the cache can hold at least 'needed' elements */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();               /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* append a candidate value to the cache */
static void append_candidate(seq_t val) {
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* public function: a(n) returns the n-th member of the sequence (1‑based) */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;              /* out of range, harmless */

    idx_t target = n - 1;      /* zero‑based index */

    /* generate terms until we have at least target+1 entries */
    while (cache_len <= target) {
        if (has_equal_zeros_and_sixes(next_candidate))
            append_candidate(next_candidate);
        ++next_candidate;
    }
    return cache[target];
}
