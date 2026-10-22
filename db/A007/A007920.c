/*
 * A007920  Smallest number k such that n + k is prime.
 * https://oeis.org/A007920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007918(n) - n; search upward from n for the first prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    uint64_t k = 0;
    while (!is_prime((uint64_t)n + k)) {
        k++;
    }
    return (seq_t)k;
}
