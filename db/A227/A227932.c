/*
 * A227932  Evil numbers (A001969) which can be written as a product of two
 *          odious numbers (A000069).
 * https://oeis.org/A227932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A227932 – Evil numbers that can be written as a product of two
 *            odious numbers
 * Range:     u8 none, u16 n <= 1040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9639 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static inline int popcount_parity(seq_t x) {
    int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;
    }
    return p;   /* 1 if odd number of ones, 0 if even */
}

static inline bool is_odious(seq_t x) {
    return popcount_parity(x) == 1;
}

static inline bool is_evil(seq_t x) {
    return popcount_parity(x) == 0;
}

static inline bool has_odious_factorization(seq_t x) {
    if ((uint64_t)x < 2) return false;
    uint64_t v = (uint64_t)x;
    uint64_t limit = (uint64_t)floor_sqrt(x);
    for (uint64_t d = 2; d <= limit; ++d) {
        if (v % d == 0) {
            uint64_t q = v / d;
            if (is_odious((seq_t)d) && is_odious((seq_t)q))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    seq_t cand = 2;
    idx_t cnt = 0;
    while (true) {
        if (is_evil(cand) && has_odious_factorization(cand)) {
            ++cnt;
            if (cnt == n)
                return cand;
        }
        ++cand;
    }
}
