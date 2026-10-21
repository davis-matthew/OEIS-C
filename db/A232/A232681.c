/*
 * A232681  Numbers n such that the equation a^2 + 5*n*b^2 = 5*c^2 + n*d^2 has
 *          no solutions in positive integers for a, b, c, d.
 * https://oeis.org/A232681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   has_qualifying_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A232681
 *
 *  Numbers n such that the equation a^2 + 5*n*b^2 = 5*c^2 + n*d^2 has
 *  no solutions in positive integers for a,b,c,d.
 *
 *  A number n belongs to the sequence iff its square‑free part contains a
 *  prime divisor p ≡ 2 or 3 (mod 5) or p = 2.  Equivalently, there is a prime
 *  factor p of n for which (5/p) = –1 with an odd exponent in the factor
 * isation of n.  The first term is 2 (offset 1).
 *
 *  The algorithm iterates over natural numbers, tests the above property
 *  by trial division, and returns the nth element.  For n ≲ 10 000 the
 *  longest candidate to test is below 20 000, so the routine runs well
 *  within the time limit.
 *
 *  The function is independent of seq_t and idx_t types supplied by the
 *  build system.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_qualifying_prime.h"

/* Main sequence function */
seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2; /* first term */

    while (1) {
        if (has_qualifying_prime(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate++;
    }
}
