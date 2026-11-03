/*
 * A106095  Primes with maximal digit = 7.
 * https://oeis.org/A106095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1389, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9704 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th prime whose decimal representation
 * uses only digits 0‑7 and contains at least one digit 7.
 * (i.e. the maximum digit appearing in the prime is exactly 7)
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* helper: true iff x contains only digits 0-7 and at least one 7 */
static bool has_max_digit_7(uint64_t x)
{
    bool has7 = false;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d == 7) has7 = true;
        if (d > 7)  return false;
        x /= 10;
    }
    return has7;
}

seq_t a(idx_t n)
{
    /* offset is 1: a(1)=7 */
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    idx_t   count   = 0;          /* number of primes found so far */
    uint64_t cand   = 7;          /* first candidate */
    while (true) {
        if (has_max_digit_7(cand) && is_prime_u64(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;                    /* next integer */
    }
}
