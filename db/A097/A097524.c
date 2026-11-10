/*
 * A097524  Duplicate of A073703.
 * https://oeis.org/A097524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = least prime p such that p + 2*prime(n) is also prime
 *            (search)
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    uint64_t d = 2 * (uint64_t)nth_prime(n);
    for (uint64_t p = 3;; p += 2) {
        if (is_prime_u64(p) && is_prime_u64(p + d))
            return (seq_t)p;
    }
}
