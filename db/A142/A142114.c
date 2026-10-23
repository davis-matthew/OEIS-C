/*
 * A142114  Primes congruent to 5 mod 37.
 * https://oeis.org/A142114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 5 modulo 37.
   For n >= 1, returns the prime p such that p % 37 == 5 and
   p is the n-th such prime in ascending order. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, but guard against 0-index call */
        return 0;

    /* n is small enough for 64‑bit arithmetic for the limits tested. */
    idx_t target     = n;
    idx_t candidate  = 5;   /* first number congruent to 5 mod 37 */
    idx_t count      = 0;

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == target)
                return (seq_t)candidate;
        }
        /* advance to next number congruent to 5 mod 37 */
        candidate += 37;
        /* protect against unsigned overflow – should never be hit for the
           values tested (max n ~ 10000). */
        if (candidate < 37)
            break;
    }
    return 0; /* unreachable for valid n, but keep compiler happy */
}
