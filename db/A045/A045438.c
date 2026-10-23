/*
 * A045438  Primes congruent to {0, 1, 4, 5, 6} mod 7.
 * https://oeis.org/A045438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 0,1,4,5,6 (mod 7) (search) */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 is assumed; for n == 0 an undefined value is returned */
    if (n == 0) return (seq_t)0;

    idx_t count    = 0;
    idx_t candidate = 2;              /* start from the first prime */

    for (;; ++candidate)
    {
        if (!is_prime(candidate))
            continue;

        idx_t mod = candidate % 7;
        if (mod == 2 || mod == 3)    /* ignore primes 2,3 mod 7 */
            continue;

        ++count;
        if (count == n)
            return (seq_t)candidate; /* cast/construct seq_t from uint64_t */
    }
}
