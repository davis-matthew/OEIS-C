/*
 * A020446  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly ten 1's.
 * https://oeis.org/A020446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 3312, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the period of the continued fraction
 * for sqrt(k) contains exactly ten 1's.  The function computes the
 * n‑th such number (n ≥ 1).  It is written to compile as C11 and
 * C++17.  In C it uses malloc/realloc; in C++ a std::vector is used
 * because seq_t may be an arbitrary‑precision class.  The continued
 * fraction algorithm counts occurrences of 1 in the period and stops
 * when the standard terminating condition a == 2*a0 is reached.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#  include <vector>
#endif

/* Integer square root for 64‑bit unsigned integers. */
static inline uint64_t _isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

#if defined(__cplusplus)
/* C++ path: use std::vector because seq_t may be an arbitrary‑precision class. */
static std::vector<seq_t> cache;
static uint64_t next_k = 2;
#else
/* C path: use a dynamically resized array of seq_t (trivially copyable). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_k = 2;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; behaviour for n < 1 is irrelevant */

#if defined(__cplusplus)
    /* Ensure we have at least n entries in the cache. */
    while ((size_t)cache.size() < (size_t)n) {
        uint64_t k = next_k++;
        uint64_t a0 = _isqrt_u64(k);
        if (a0 * a0 == k) continue;   /* perfect square – no period */

        uint64_t m = 0, d = 1, a = a0, two_a0 = a0 << 1;
        int ones = 0;

        do {
            m = d * a - m;
            d = (k - m * m) / d;
            a = (a0 + m) / d;
            if (a == 1) ++ones;
        } while (a != two_a0);

        if (ones == 10)
            cache.push_back((seq_t)k);
    }
    return cache[n - 1];
#else
    /* C path: dynamic array of seq_t. */
    while (cache_len < n) {
        uint64_t k = next_k++;
        uint64_t a0 = _isqrt_u64(k);
        if (a0 * a0 == k) continue;   /* skip perfect squares */

        uint64_t m = 0, d = 1, a = a0, two_a0 = a0 << 1;
        int ones = 0;

        do {
            m = d * a - m;
            d = (k - m * m) / d;
            a = (a0 + m) / d;
            if (a == 1) ++ones;
        } while (a != two_a0);

        if (ones == 10) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) exit(1);  /* allocation failure */
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
#endif
}
