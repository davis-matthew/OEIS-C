/*
 * A108187  Numbers n such that 11*n - 5 is prime.
 * https://oeis.org/A108187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that 11*k - 5 is prime
 * Range:     u8 n <= 6, u16 n <= 670, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9032 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (count < n) {
        seq_t cand = (seq_t)11 * k - 5;
        if (is_prime_u64((uint64_t)cand)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    return 0;  /* unreachable */
}
