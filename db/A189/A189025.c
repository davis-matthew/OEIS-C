/*
 * A189025  Number of primes in the range (n - 2*sqrt(n), n].
 * https://oeis.org/A189025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ primes p | floor(n‑2*sqrt(n)) < p ≤ n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the count using only integer arithmetic and the helper
   functions int_sqrt(idx_t) and prime_pi(idx_t). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /*  Compute ceil(2*sqrt(n)) without floating point:
          ceil(2*sqrt(n)) = floor(sqrt(4*n)) + (is_not_perfect_square(4*n) ? 1 : 0)  */
    idx_t sqrt4 = int_sqrt(4 * n);                         /* floor(sqrt(4*n)) == floor(2*sqrt(n)) */
    bool perfect4 = (sqrt4 * sqrt4 == 4 * n);             /* 2*sqrt(n) integer?       */
    idx_t ceil2sqrt = sqrt4 + (perfect4 ? 0 : 1);          /* ceil(2*sqrt(n))          */

    /* lowerBound = floor(n - 2*sqrt(n)) = n - ceil(2*sqrt(n))  (if not underflow) */
    idx_t lower = 0;
    if (n > ceil2sqrt) {
        lower = n - ceil2sqrt;
    }

    seq_t pi_n = prime_pi(n);
    seq_t pi_lower = 0;
    if (lower >= 2) {
        pi_lower = prime_pi(lower);
    }

    return pi_n - pi_lower;
}
