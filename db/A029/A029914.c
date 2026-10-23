/*
 * A029914  Start with n; repeatedly sum squares of prime factors (counted
 *          with multiplicity), until reach a prime p, then set a(n) = p; if
 *          reach a fixed point q, set a(n) = q; set a(n) = 0 if no limit
 *          exists.
 * https://oeis.org/A029914
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big, sum_prime_factor_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029914.
 * Start with n; repeatedly replace by the sum of squares of its prime
 * factors (counted with multiplicity).  Stop when the result is a prime
 * p, then return p;  if a fixed point q is reached, return q;
 * otherwise return 0 after a reasonable number of iterations. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/sum_prime_factor_squares.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;                 /* sequence offset starts at 2 */
    }

    seq_t current = (seq_t)n;
    const int MAX_STEPS = 1024;

    for (int step = 0; step < MAX_STEPS; ++step) {
        seq_t next = sum_prime_factor_squares(current);
        if (is_prime_big(next)) {
            return next;          /* reached a prime */
        }
        if (next == current) {
            return next;          /* fixed point */
        }
        current = next;
    }
    return 0;                     /* no limit found within step bound */
}
