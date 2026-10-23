/*
 * A142190  Primes congruent to 19 mod 40.
 * https://oeis.org/A142190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 406, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 19 modulo 40 */

/* This function returns the n-th prime p such that p % 40 == 19.
   It simply iterates candidates starting from 19, stepping by 40,
   and tests each for primality using the helper is_prime().
   Since the density of such primes is roughly 1/8 of all primes,
   the n-th instance is located below about 8·n·log(8n), which is
   well below 2 million for n <= 10000, so the routine is fast. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; this value is unused but harmless */
        return 0;

    seq_t candidate = 19;
    idx_t count = 0;

    while (1) {
        if (is_prime((idx_t)candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += 40;
    }
}
