/*
 * A103409  Numbers n such that n2357 is prime.
 * https://oeis.org/A103409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 8552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th integer k such that the decimal concatenation
   of k followed by "2357" is a prime number.
   The sequence is defined as: a(n) = k where k*10000 + 2357 is prime. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* offset 1: a(1) = 0, so we count from 0 upward */
    idx_t count = 0;
    idx_t k = 0;
    while (true) {
        /* form the integer k*10000 + 2357 */
        uint64_t candidate = (uint64_t)k * 10000ULL + 2357ULL;
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                /* n fits into seq_t by construction */
                return (seq_t)k;
            }
        }
        ++k;
    }
}
