/*
 * A089986  Numbers n such that 4n + 7 is prime.
 * https://oeis.org/A089986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 51, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 4*n + 7 is prime.
   a(1) = -1.  For n ≥ 2, a(n) is the (n‑2)-th non‑negative integer m
   with 4*m + 7 prime.  The function uses the helper is_prime_u64
   to test primality of the 64‑bit candidate.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* The very first term corresponds to n = –1. */
    if (n == 1) {
        return (seq_t)-1;
    }

    /* For n ≥ 2 we search for the (n‑2)-th non‑negative m
       that satisfies that 4*m + 7 is prime. */
    idx_t target = n - 2;   /* zero‑based index of the needed prime */
    idx_t count  = 0;       /* number of primes found so far   */
    idx_t m      = 0;       /* current non‑negative integer    */

    for (;; ++m) {
        uint64_t val = (uint64_t)4 * m + 7;
        if (is_prime_u64(val)) {
            if (count == target) {
                return (seq_t)m;
            }
            ++count;
        }
    }
}
