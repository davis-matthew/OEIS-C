/*
 * A000977  Numbers that are divisible by at least three different primes.
 * https://oeis.org/A000977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   nth number divisible by at least three distinct primes
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9358 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/omega.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2;
    
    while (1) {
        if (omega(candidate) >= 3) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate++;
    }
}
