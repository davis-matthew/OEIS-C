/*
 * A073481  Least prime factor of the n-th squarefree number.
 * https://oeis.org/A073481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime factor of the n-th squarefree number
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree_local.h"

// Helper: return the least prime factor of x; 1 if x <= 1
static seq_t smallest_prime_factor_uint64(uint64_t x) {
    if (x <= 1) return (seq_t)1;
    if (x % 2 == 0) return (seq_t)2;
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) return (seq_t)p;
    }
    return (seq_t)x;          // x is prime
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;  // a(1) = 1

    idx_t count = 0;
    uint64_t num = 0;

    while (count < n) {
        ++num;
        if (is_squarefree_local(num)) {
            ++count;
            if (count == n) {
                return smallest_prime_factor_uint64(num);
            }
        }
    }

    // Should never reach here for n >= 1
    return (seq_t)0;
}
