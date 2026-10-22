/*
 * A003799  Order of simple Chevalley group A_n (9).
 * https://oeis.org/A003799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(n(n+1)/2) * Product_{k=2..n+1} (9^k - 1) / gcd(n+1, 8)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   pow9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow9.h"

static uint64_t gcd_u64(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = y;
        y = x % y;
        x = t;
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t e = n * (n + 1) / 2;
    seq_t result = pow9(e);
    uint64_t g = gcd_u64(n + 1, 8);
    seq_t p = 9;
    for (idx_t k = 2; k <= n + 1; k++) {
        p *= 9;
        seq_t f = p - 1;
        /* 9^2 - 1 = 80 is divisible by 8, so divide exactly here */
        if (k == 2) f /= (seq_t)g;
        result *= f;
    }
    return result;
}
