/*
 * A142120  Primes congruent to 11 mod 37.
 * https://oeis.org/A142120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 187, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Return the nth prime that is congruent to 11 (mod 37).
   This is done by iterating over the arithmetic progression 11 + 37*k
   and checking each term for primality with the supplied is_prime()
   routine. The first prime found is the first term, the second prime
   found is the second term, and so on. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n>=1.  For n==0 we return 0
       (value does not matter outside the offset). */
    if (n == 0) return 0;

    idx_t candidate = 11;   /* first number congruent to 11 mod 37 */
    idx_t count = 0;        /* how many such primes have been found */

    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 37;
    }
}
