/*
 * A018252  The nonprime numbers: 1 together with the composite numbers,
 *          A002808.
 * https://oeis.org/A018252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, core
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 9361, u32 n <= 9361, u64 n <= 9361,
 *            u128 n <= 9361, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018252: the non‑prime numbers (including 1).
   a(1) = 1, a(2) = 4, a(3) = 6, … */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Return the nth non‑prime number.  If n < 1, the behaviour
       is undefined – the offset of the sequence is 1. */
    if (n < 1) return 0;     /* safety, should not be called with n < 1 */

    idx_t count = 0;         /* how many non‑primes seen so far */
    seq_t candidate = 1;     /* start with 1, which is non‑prime */

    while (1) {
        /* is_prime accepts idx_t; cast candidate accordingly. */
        if (!is_prime((idx_t)candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
