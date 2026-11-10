/*
 * A055197  Numbers k such that A005728(k) is not prime.
 * https://oeis.org/A055197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search: k such that A005728(k) = 1 + Sum_{i=1..k} phi(i) is not
 *            prime; running sum kept in 64-bit integers
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9538 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static uint64_t A005728_phi(uint64_t n) {
    if (n == 1) return 1;
    uint64_t result = n;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result = result / p * (p - 1);
        }
    }
    if (temp > 1) result = result / temp * (temp - 1);
    return result;
}

seq_t a(idx_t n) {
    uint64_t sum = 1;
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        k++;
        sum += A005728_phi((uint64_t)k);
        if (!is_prime((idx_t)sum)) count++;
    }
    return (seq_t)k;
}
