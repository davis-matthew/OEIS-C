/*
 * A050813  Numbers n not palindromic in any base b, 2 <= b <= 10.
 * https://oeis.org/A050813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer that is NOT a palindrome in any
 *            base b, 2 ≤ b ≤ 10
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_nonpalin_all_bases, is_pal_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_nonpalin_all_bases.h"
#include "../utils/is_pal_in_base.h"

/* Memoised sequence storage.  The array holds the terms a(1), a(2), … .
   The static variables keep state between calls. */
static seq_t *seq_cache = NULL;     /* array of stored terms */
static size_t seq_capacity = 0;     /* allocated elements */
static idx_t seq_count = 0;         /* number of stored terms */
static seq_t last_value = 0;        /* last value added to the cache */

/* Ensure the cache can hold at least 'needed' terms.  Grow exponentially. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= seq_capacity) {
        return;
    }
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
    while (new_cap < needed) {
        new_cap *= 2;
    }
    seq_t *new_ptr = (seq_t *) realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a real build we would handle allocation failure.  For this
           evaluator we simply abort. */
        abort();
    }
    seq_cache = new_ptr;
    seq_capacity = new_cap;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    /* n is 1‑based according to OEIS offset. */
    if (n == 0) {
        /* Behavior below offset does not matter per requirement. */
        return 0;
    }

    if (n <= seq_count) {
        return seq_cache[n - 1];
    }

    /* We need to compute terms up to index n. */
    ensure_cache_size(n);

    seq_t cand = last_value + 1;  /* start searching after the last cached value */
    for (idx_t idx = seq_count; idx < n; ++idx) {
        /* Find the next qualifying number. */
        while (!is_nonpalin_all_bases(cand)) {
            ++cand;
        }
        seq_cache[idx] = cand;
        last_value = cand;
        ++cand;
    }
    seq_count = n;
    return seq_cache[n - 1];
}
