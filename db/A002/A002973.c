/*
 * A002973  a(n) is half of the even member of {x,y}, where x^2 + y^2 is the
 *          n-th prime of the form 4i+1.
 * https://oeis.org/A002973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is half of the even member of {x,y}, where x^2 + y^2 is the
 *            n-th prime of the form 4i+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    
    uint64_t i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

static uint64_t isqrt_u64(uint64_t x) {
    if (x == 0) return 0;
    uint64_t s = x;
    uint64_t t = (s + 1) / 2;
    while (t < s) {
        s = t;
        t = (s + x / s) / 2;
    }
    return s;
}

static uint64_t find_even_half(uint64_t p) {
    for (uint64_t x = 1; x * x < p; x++) {
        uint64_t y_sq = p - x * x;
        uint64_t y = isqrt_u64(y_sq);
        if (y * y == y_sq) {
            if (x % 2 == 0) {
                return x / 2;
            } else {
                return y / 2;
            }
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t candidate = 5;
    
    while (true) {
        if (is_prime(candidate) && candidate % 4 == 1) {
            count++;
            if (count == n) {
                return (seq_t) find_even_half(candidate);
            }
        }
        candidate += 2;
    }
}
