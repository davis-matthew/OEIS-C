/*
 * A005122  Numbers k such that 8k - 1 is prime.
 * https://oeis.org/A005122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th k >= 1 such that 8k - 1 is prime
 *            (trial-division test)
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 964 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    uint64_t k = 0;
    idx_t count = 0;
    while (count < n) {
        k++;
        if (is_prime_u64(8 * k - 1)) count++;
    }
    return (seq_t)k;
}
