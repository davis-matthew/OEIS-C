/*
 * A004117  Numerators of expansion of (1-x)^(-1/3).
 * https://oeis.org/A004117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * (3n-2) / m where n = 3^v * m with 3 not dividing
 *            m, a(0) = 1 (equivalently a(n) = 3^A054861(n) *
 *            Product_{k=0..n-1} (3k+1) / n!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *memo = 0;
static idx_t memo_len = 0;
static idx_t memo_cap = 0;

static uint64_t gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (memo_len == 0) {
        memo_cap = 16;
        memo = grow_zero(memo, 0, memo_cap);
        memo[0] = 1;
        memo_len = 1;
    }
    if (n >= memo_cap) {
        idx_t newcap = memo_cap;
        while (newcap <= n) newcap *= 2;
        memo = grow_zero(memo, memo_cap, newcap);
        memo_cap = newcap;
    }
    while (memo_len <= n) {
        idx_t k = memo_len;
        uint64_t m = k;
        while (m % 3 == 0) m /= 3;
        seq_t prev = memo[k - 1];
        uint64_t r = (uint64_t)(prev % (seq_t)m);
        uint64_t g = gcd_u64(m, r);
        uint64_t mg = m / g;
        uint64_t f = (3 * (uint64_t)k - 2) / mg;
        memo[k] = (prev / (seq_t)g) * (seq_t)f;
        memo_len++;
    }
    return memo[n];
}
