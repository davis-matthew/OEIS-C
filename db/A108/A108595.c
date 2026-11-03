/*
 * A108595  Numbers k such that 10*k + 103 is prime.
 * https://oeis.org/A108595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that 10*k+103 is prime; a(1)=0
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9597 (2026-11-03)
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
    if (n == 0) return 0;               /* undefined for offset 1, guard */
    idx_t count = 0;
    idx_t k = 0;
    while (true) {
        /* 10*k+103 fits in 64 bits for all n of interest (n ≤ 10,000). */
        uint64_t candidate = 10 * (uint64_t)k + 103;
        if (is_prime_u64(candidate)) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
}
