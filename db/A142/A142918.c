/*
 * A142918  Primes congruent to 52 mod 63.
 * https://oeis.org/A142918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p % 63 == 52 (a(1) = 241, a(2)
 *            = 367, …)
 * Range:     u8 n <= 1, u16 n <= 190, u32 n <= 59, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1; return 0 for safety. */
    if (n < 1) return 0;

    idx_t count = 0;          /* number of suitable primes found so far */
    idx_t k = 1;              /* iterator over the arithmetic progression 63*k+52 */

    while (true) {
        /* Compute the next candidate in 64‑bit arithmetic to avoid overflow
           of the intermediate product. */
        uint64_t temp = 63ULL * (uint64_t)k + 52ULL;
        seq_t candidate = (seq_t)temp;

        /* Test primality using the provided generic primality test. */
        if (is_prime_big(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        k++;
    }
}
