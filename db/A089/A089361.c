/*
 * A089361  Numbers of pairs (i, j), i, j > 1, such that i^j <= n.
 * https://oeis.org/A089361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=2..floor(log_2(n))} floor(n^(1/j) - 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: check whether base^exp <= limit, using 128‑bit arithmetic */
static int pow_leq(idx_t base, idx_t exp, idx_t limit) {
    __uint128_t prod = 1;
    for (idx_t i = 0; i < exp; ++i) {
        if (prod > (__uint128_t)limit / base) return 0;
        prod *= base;
    }
    return prod <= limit;
}

/* Helper: compute floor(n^(1/exp)),  exp >= 1 */
static idx_t floor_root(idx_t n, idx_t exp) {
    idx_t low = 1, high = n, best = 1;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (pow_leq(mid, exp, n)) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;       /* safety against underflow */
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t result = 0;

    __uint128_t pow2 = (__uint128_t)4;   /* 2^2 */
    for (idx_t j = 2; pow2 <= n; ++j, pow2 <<= 1) {
        idx_t root = floor_root(n, j);
        if (root < 2) break;
        result += (seq_t)(root - 1);
    }
    return result;
}
