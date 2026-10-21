/*
 * A057531  Numbers whose sum of digits and number of divisors are equal.
 * https://oeis.org/A057531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n-th positive integer x such that digit_sum(x) ==
 *            divisor_count(x)
 * Range:     u8 n <= 14, u16 n <= 1163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_realloc.h"

/* Local helper to compute sum of decimal digits of a 64‑bit integer */
static unsigned int digit_sum_uint64(uint64_t v)
{
    unsigned int s = 0;
    while (v) {
        s += (unsigned int)(v % 10);
        v /= 10;
    }
    return s;
}

/* Memoised generator for A057531 */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t   checked_up_to = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Out of range, behaviour not defined */

    while (cache_size < (size_t)n) {
        ++checked_up_to;
        uint64_t cand = (uint64_t)checked_up_to;

        /* Compute digit sum */
        unsigned int ds = digit_sum_uint64(cand);

        /* Compute number of divisors (tau) */
        seq_t tau = divisor_count_u64(cand);

        if (ds == (unsigned int)tau) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[(size_t)n - 1];
}
