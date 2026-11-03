/*
 * A044859  Positive integers having the same number of base-3 runs of odd
 *          length as even.
 * https://oeis.org/A044859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) enumerates the positive integers whose base‑3
 *            representation * contains an equal number of runs of odd length
 *            and runs of even length
 * Range:     u8 n <= 26, u16 n <= 3324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9870 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
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

static bool runs_equal(seq_t x) {
    if (x == 0) return false;          /* positive integers only */
    seq_t digit = x % 3;
    x /= 3;
    unsigned long long run_len = 1ULL;
    unsigned even = 0, odd = 0;

    while (x > 0) {
        seq_t next_digit = x % 3;
        if (next_digit == digit) {
            run_len++;
        } else {
            if ((run_len & 1ULL) == 0ULL) even++; else odd++;
            digit = next_digit;
            run_len = 1ULL;
        }
        x /= 3;
    }
    /* account for the final run */
    if ((run_len & 1ULL) == 0ULL) even++; else odd++;

    return even == odd;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n = 0, but harmless */

    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;

    /* Grow the cache until it contains the nth term */
    while (cache_len < n) {
        seq_t val = (seq_t)next_candidate;

        if (runs_equal(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        next_candidate++;   /* next integer to check */
    }

    return cache[n - 1];
}
