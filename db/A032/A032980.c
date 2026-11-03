/*
 * A032980  Numbers with the property that all pairs of consecutive base-9
 *          digits differ by 0 or 1.
 * https://oeis.org/A032980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 870, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 880 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A032980: numbers whose base‑9 digits differ by 0 or 1 between
   consecutive digits. Uses brute‑force enumeration with memoization. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static inline bool valid_base9_adj(seq_t val)
{
    /* Numbers with fewer than two base‑9 digits are automatically valid. */
    if (val < 9) return true;

    seq_t temp = val;
    seq_t prev = temp % 9;
    temp /= 9;
    while (temp > 0) {
        seq_t curr = temp % 9;
        /* Ensure |prev – curr| <= 1. */
        if ((prev > curr ? prev - curr : curr - prev) > (seq_t)1)
            return false;
        prev = curr;
        temp /= 9;
    }
    return true;
}

static seq_t *cache = NULL;      /* Array of previously found terms. */
static size_t  cache_len = 0;    /* Number of stored terms. */
static size_t  cache_cap = 0;    /* Allocated capacity. */
static seq_t   next_candidate = 1;  /* Next number to test. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined, but harmless. */

    /* Expand cached sequence until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next valid number. */
        while (true) {
            if (valid_base9_adj(next_candidate)) {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();          /* Allocation failed. */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
                break;
            }
            ++next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
