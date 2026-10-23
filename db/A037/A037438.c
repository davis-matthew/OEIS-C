/*
 * A037438  Positive numbers having the same set of digits in base 7 and base
 *          8.
 * https://oeis.org/A037438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 385, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A037438: positive numbers having the same set of digits in base 7 and base 8. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"

static inline bool same_digit_set(seq_t x)
{
    if (x == 0) return false;               /* 0 is not in the sequence */
    unsigned umask7 = 0, umask8 = 0;
    seq_t t = x;

    /* digits in base 7 */
    while (t > 0) {
        unsigned d = (unsigned)(t % 7);
        umask7 |= 1u << d;
        t /= 7;
    }

    t = x;
    /* digits in base 8; digit 7 must not appear */
    while (t > 0) {
        unsigned d = (unsigned)(t % 8);
        if (d == 7) return false;
        umask8 |= 1u << d;
        t /= 8;
    }

    return umask7 == umask8;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t next_candidate = 1;

    if (n == 0) return 0;   /* offset not defined for 0 */

    while (cache_len < (size_t)n) {
        if (same_digit_set(next_candidate)) {
            append_candidate(&cache, &cache_len, &cache_cap, next_candidate);
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
