/*
 * A045346  Primes congruent to {0, 1, 2, 4, 5, 6} mod 7.
 * https://oeis.org/A045346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime congruent to 0,1,2,4,5,6 (i.e. not ≡ 3) mod 7
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {    /* the sequence is 1-indexed; return 0 for n=0 if called */
        return (seq_t)0;
    }

    idx_t count   = 0;
    idx_t candidate = 2;

    while (true) {
        if (is_prime(candidate) && (candidate % 7 != 3)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
