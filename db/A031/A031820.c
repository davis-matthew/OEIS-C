/*
 * A031820  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly 52 ones.
 * https://oeis.org/A031820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that the period of the continued fraction for *
 *            sqrt(k) contains exactly 52 instances of the partial quotient 1.
 *            * (Computed by brute‑force search.)
 * Range:     u8 none, u16 n <= 592, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Helper: floor(sqrt(x)) for 64‑bit unsigned integers. */
static idx_t isqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t low = 0, high = x >> 1;   /* high <= 2^63-1 so high*high fits in 128‑bit */
    idx_t mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
            res = mid;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* Test whether k has exactly 52 ones in the period of the continued
 * fraction expansion of sqrt(k). */
static bool k_has_52_ones(idx_t k)
{
    if (k == 0) return false;
    idx_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* perfect square */

    idx_t m = 0, d = 1, a = a0;
    int ones = 0;

    for (;;) {
        /* Compute next state using 128-bit intermediates to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)d * a;
        idx_t m_next = (idx_t)(prod - m);

        unsigned __int128 m_sq = (unsigned __int128)m_next * m_next;
        idx_t d_next = (idx_t)((k - (uint64_t)m_sq) / d);

        idx_t a_next = (a0 + m_next) / d_next;

        if (a_next == 1) {
            ++ones;
            if (ones > 52) return false;    /* early bail */
        }

        if (a_next == 2 * a0) {
            break;                          /* period ends */
        }

        m = m_next;
        d = d_next;
        a = a_next;
    }
    return ones == 52;
}

/* Main sequence function with lazy memoisation. */
seq_t a(idx_t n)
{
    /* Handle negative or zero n safely; offset is 1. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache    = NULL;
    static size_t cache_sz = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t next_k = 2;      /* start checking from 2 */

    while (cache_sz < (size_t)n) {
        /* Find next k satisfying the condition. */
        while (!k_has_52_ones(next_k)) {
            ++next_k;
        }

        /* Ensure capacity. */
        if (cache_sz == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
}
