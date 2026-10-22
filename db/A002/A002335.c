/*
 * A002335  Least positive integer y such that A038873(n) = x^2 - 2y^2 for
 *          some x.
 * https://oeis.org/A002335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for the n-th prime p where p=2 or p≡1,7 (mod 8), find least y
 *            such that p = x^2 - 2y^2
 * Range:     u8 n <= 6223, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

static uint64_t isqrt_u64(uint64_t x) {
    if (x < 2) return x;
    uint64_t y = x;
    uint64_t z = (y + 1) / 2;
    while (z < y) {
        y = z;
        z = (y + x / y) / 2;
    }
    return y;
}

static seq_t least_y_for_prime(uint64_t p) {
    for (uint64_t y = 1; y <= 100000; y++) {
        uint64_t x_sq = p + 2 * y * y;
        uint64_t x = isqrt_u64(x_sq);
        if (x * x == x_sq) {
            return (seq_t)y;
        }
    }
    return (seq_t)0;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 2;
    
    while (count < n) {
        if (is_prime(p)) {
            uint64_t r = (p == 2) ? 2 : (p % 8);
            if (r == 1 || r == 2 || r == 7) {
                count++;
                if (count == n) {
                    return least_y_for_prime(p);
                }
            }
        }
        p += (p == 2) ? 1 : 2;
    }
    
    return (seq_t)0;
}
