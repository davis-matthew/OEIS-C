/*
 * A013649  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly two 1's.
 * https://oeis.org/A013649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that the period of the continued
 *            fraction for sqrt(k) contains exactly two 1's
 * Range:     u8 n <= 67, u16 n <= 5177, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n < 2^64 */
static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1 << 32);
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether n is a perfect square */
static inline bool is_square(uint64_t n) {
    uint64_t r = isqrt_u64(n);
    return ((unsigned __int128)r * r) == n;
}

/* Return true if the period of continued fraction for sqrt(k) contains exactly two 1's */
static bool has_two_ones(uint64_t k) {
    uint64_t a0 = isqrt_u64(k);
    if ((unsigned __int128)a0 * a0 == k)
        return false;                     /* k is a perfect square */

    uint64_t m = 0, d = 1, a = a0;
    unsigned count = 0;

    while (1) {
        uint64_t m_next = (uint64_t)(((unsigned __int128)d * a) - m);
        uint64_t m_next_sq = m_next * m_next;
        uint64_t d_next = (k - m_next_sq) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        if (a_next == 1)
            ++count;
        if (a_next == 2 * a0)
            break;                         /* period completed */

        m = m_next;
        d = d_next;
        a = a_next;
    }

    return count == 2;
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 1;

    while (cache_len < n) {
        ++next_k;
        if (is_square(next_k))
            continue;
        if (has_two_ones(next_k)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
    }

    return cache[n - 1];
}
