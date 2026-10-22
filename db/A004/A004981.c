/*
 * A004981  a(n) = (2^n/n!) * Product_{k=0..n-1} (4*k + 1).
 * https://oeis.org/A004981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   n*a(n) = 2*(4*n-3)*a(n-1), a(0) = 1; computed as a(n) =
 *            (a(n-1)/g) * (2*(4*n-3)/(n/g)) with g = gcd(a(n-1), n) to avoid
 *            overflow
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_CACHED 10001

static seq_t cache[MAX_CACHED];
static idx_t max_computed = 0;

static uint64_t gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t step(seq_t prev, idx_t i) {
    seq_t si = (seq_t)i;
    uint64_t g = 1;
    if (si != 0) {
        uint64_t r = (uint64_t)(prev % si);
        g = gcd_u64(i, r);
    }
    uint64_t d = i / g;
    uint64_t m = 2 * (4 * (uint64_t)i - 3);
    seq_t q = prev / (seq_t)g;
    return q * (seq_t)(m / d);
}

seq_t a(idx_t n) {
    idx_t target = (n < MAX_CACHED) ? n : (MAX_CACHED - 1);

    while (max_computed <= target) {
        cache[max_computed] = (max_computed == 0) ? (seq_t)1
                              : step(cache[max_computed - 1], max_computed);
        max_computed++;
    }

    if (n < MAX_CACHED) {
        return cache[n];
    }

    seq_t result = cache[MAX_CACHED - 1];
    for (idx_t i = MAX_CACHED; i <= n; i++) {
        result = step(result, i);
    }
    return result;
}
