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
 * Helpers:   alloc_seq, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"
#include "../utils/seq_realloc.h"

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
