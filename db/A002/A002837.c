/*
 * A002837  Numbers k such that k^2 - k + 41 is prime.
 * https://oeis.org/A002837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = k such that k^2 - k + 41 is prime
 * Range:     u8 n <= 188, u16 n <= 8946, u32 n <= 8946, u64 n <= 8946,
 *            u128 n <= 8946, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    if (n < 9) return true;
    if ((n % 3) == 0) return false;
    
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if ((n % i == 0) || (n % (i + 2) == 0))
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    
    while (true) {
        uint64_t val = (uint64_t)k * k - k + 41;
        if (is_prime_u64(val)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
