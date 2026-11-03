/*
 * A001101  Moran numbers: k such that k/(sum of digits of k) is prime.
 * https://oeis.org/A001101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9981 (2026-11-01)
 * Helpers:   digit_sum_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum_u64.h"

/* Search for k such that k/(sum of digits of k) is prime */

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

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    
    while (count < n) {
        k++;
        uint64_t ds = digit_sum_u64(k);
        if (ds > 0 && k % ds == 0) {
            uint64_t quotient = k / ds;
            if (is_prime(quotient)) {
                count++;
            }
        }
    }
    
    return (seq_t)k;
}
