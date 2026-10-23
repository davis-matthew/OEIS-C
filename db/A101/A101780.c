/*
 * A101780  Primes of the form 100*n + 3.
 * https://oeis.org/A101780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form 100*k + 3 by iterating candidates
   and testing primality using the provided is_prime_seq helper. */
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        /* candidate = 100 * k + 3 using seq_t arithmetic */
        seq_t candidate = (seq_t)100 * (seq_t)k + (seq_t)3;

        if (is_prime_seq(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++k;
    }
}
