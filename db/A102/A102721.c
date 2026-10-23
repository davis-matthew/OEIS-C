/*
 * A102721  Numbers n such that 11*n + 13 is prime.
 * https://oeis.org/A102721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer x with 11*x + 13 prime
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 11*n + 13 is prime */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"   /* provides bool is_prime_uint64(uint64_t) */

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;                     /* candidate n */

    while (1) {
        uint64_t val = 11ULL * (uint64_t)m + 13ULL;   /* 11*m + 13 */
        if (is_prime_uint64(val)) {
            count++;
            if (count == n) {
                return (seq_t)m;
            }
        }
        m++;
    }
}
