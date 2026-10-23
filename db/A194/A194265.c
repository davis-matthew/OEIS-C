/*
 * A194265  Smallest product of two distinct primes of the form n*k+1.
 * https://oeis.org/A194265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = product of the two smallest distinct primes that are ≡ 1 (mod n). */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    /* Convert n to seq_t for arithmetic, but keep its original value for loop step. */
    seq_t step = (seq_t)n;

    seq_t candidate = step + (seq_t)1; /* first number congruent to 1 mod n */
    seq_t first = 0, second = 0;

    /* Find the two smallest distinct primes ≡ 1 (mod n). */
    while (first == 0 || second == 0) {
        if (is_prime_small(candidate)) {
            if (first == 0)
                first = candidate;
            else if (second == 0 && candidate != first)
                second = candidate;
        }
        candidate += step;
    }

    return first * second;
}
