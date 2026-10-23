/*
 * A106095  Primes with maximal digit = 7.
 * https://oeis.org/A106095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1389, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_max_digit_7, is_prime
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
#include "../utils/is_prime.h"   /* declaration: bool is_prime(uint64_t) */
#include "../utils/has_max_digit_7.h"

seq_t a(idx_t n)
{
    /* offset is 1: a(1)=7 */
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    idx_t   count   = 0;          /* number of primes found so far */
    uint64_t cand   = 7;          /* first candidate */
    while (true) {
        if (has_max_digit_7(cand) && is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;                    /* next integer */
    }
}
