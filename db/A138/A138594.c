/*
 * A138594  Sums of 6 or more consecutive nonnegative integers.
 * https://oeis.org/A138594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9320 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138594: Sums of 6 or more consecutive nonnegative integers. */
/* a(n) = the n‑th integer that can be written as k*s + k*(k-1)/2 for some integer k ≥ 6. */
/* The function builds the list lazily, storing already found terms in a static array. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline bool has_rep(seq_t sum)
{
    /* Test whether 'sum' can be written as k*s + k*(k-1)/2 with k ≥ 6. */
    for (idx_t k = 6; ; ++k) {
        seq_t start = (seq_t)k * (k - 1) / 2;
        if (start > sum) break;
        if ((sum - start) % k == 0) return true;
    }
    return false;
}

/* The array below is large enough for all required n (up to ~10 000).  */
#define MAX_CACHE 30000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;     /* number of terms already stored */
static idx_t next_candidate = 15;/* integers to test, starting at the first possible sum */

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n stored values. */
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        if (has_rep(val)) {
            if (cache_len < MAX_CACHE) {
                cache[cache_len++] = val;
            } else {
                /* In the unlikely event we exceed the preallocated array,
                   we stop computing further terms, as the caller is expected
                   to request only feasible indices. */
                break;
            }
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
