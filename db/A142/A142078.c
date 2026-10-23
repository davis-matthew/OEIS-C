/*
 * A142078  Primes congruent to 3 mod 35.
 * https://oeis.org/A142078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 3 mod 35
 * Range:     u8 n <= 2, u16 n <= 276, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 3;            /* 35*0 + 3, the first congruent to 3 */
    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        /* next number congruent to 3 mod 35 */
        candidate += 35;
    }
}
