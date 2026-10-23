/*
 * A020444  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly eight 1's.
 * https://oeis.org/A020444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 3466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Computes the set of integers k for which the simple continued‑fraction
   expansion of sqrt(k) has a repeating block that contains exactly eight
   partial quotients equal to 1.  a(n) is the n‑th such integer (1‑based). */

static uint64_t isqrt64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1ULL << 32; /* 2^32 > sqrt(2^64)            */
    while (high > low) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static bool has_eight_ones(uint64_t k)
{
    uint64_t a0 = isqrt64(k);
    if (a0 * a0 == k)                /* skip perfect squares */
        return false;

    uint64_t m = 0, d = 1, a = a0;
    uint64_t count = 0;

    do {
        unsigned __int128 prod = (unsigned __int128)d * a;
        uint64_t m_next = (uint64_t)(prod - m);
        uint64_t d_next = (k - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        if (a_next == 1) {
            ++count;
            if (count > 8)
                return false;      /* early exit */
        }

        m = m_next;
        d = d_next;
        a = a_next;
    } while (a != 2 * a0);

    return count == 8;
}

/* Cache of previously found terms */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t next_k   = 2;       /* integer to test next          */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 1       */

    size_t target = (size_t)n;
    while (cache_size < target) {
        for (;;) {
            uint64_t r = isqrt64(next_k);
            if (r * r != next_k) {               /* not a perfect square */
                if (has_eight_ones(next_k)) {    /* qualifies          */
                    if (cache_size >= cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                       new_cap * sizeof(seq_t));
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_size++] = (seq_t)next_k;
                    ++next_k;
                    break;            /* move to next candidate    */
                }
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
