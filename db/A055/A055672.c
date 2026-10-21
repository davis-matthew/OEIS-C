/*
 * A055672  Number of right-inequivalent prime Hurwitz quaternions of norm n.
 * https://oeis.org/A055672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000593(n) * A010051(n) For this sequence this simplifies
 *            to: a(p) = p + 1 for odd primes p a(2) = 1 a(n) = 0 otherwise
 *            (where n=0 or 1 also give 0)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 2)                // a(0) = a(1) = 0
        return 0;

    if (is_prime(n)) {        // prime case
        if (n == 2)
            return 1;        // a(2) = 1
        /* for odd primes we add 1 */
        return (seq_t)n + 1;
    }

    return 0;                 // composite n
}
