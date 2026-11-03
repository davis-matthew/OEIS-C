/*
 * A044844  Positive integers having no fewer base-3 runs of even length than
 *          odd.
 * https://oeis.org/A044844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044844: positive integers whose number of runs of even length
   in base‑3 is not fewer than the number of runs of odd length.  The
   following function enumerates integers in increasing order and selects
   those that satisfy the property.  It stores the first terms in a static
   cache, so that arbitrary (non‑monotonic) queries are answered in
   amortised O(1) time. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
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

/* Return true if integer x satisfies the run‑length condition. */
static bool has_valid_base3_runs(seq_t x)
{
    unsigned runlen = 0;
    int prev_digit = -1;
    int r_even = 0, r_odd = 0;

    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)3);
        if (d != prev_digit) {
            if (runlen) {
                if ((runlen & 1) == 0) r_even++;
                else r_odd++;
            }
            prev_digit = d;
            runlen = 1;
        } else {
            runlen++;
        }
        x /= (seq_t)3;
    }

    if (runlen) {
        if ((runlen & 1) == 0) r_even++;
        else r_odd++;
    }
    return r_even >= r_odd;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 1;

/* Ensure the cache has capacity for at least 'needed' terms. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = (cache_cap == 0) ? 16 : cache_cap;
    while (newcap < needed) newcap <<= 1;
    cache = seq_realloc(cache, cache_cap, newcap);
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; the sequence has offset 1. */
    if (n == (idx_t)0) return (seq_t)0;

    while (cache_len < n) {
        if (has_valid_base3_runs(last_checked)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
        last_checked++;
    }
    return cache[n - 1];
}
