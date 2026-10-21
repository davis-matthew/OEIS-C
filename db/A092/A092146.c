/*
 * A092146  Primes of the form p + 10 where p is a prime.
 * https://oeis.org/A092146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by checking primes p and testing whether p+10 is prime. */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined offset 0, return 0 */

    idx_t target = n;                /* number of terms we need to find */
    idx_t found  = 0;                /* how many qualifying primes we have found */
    idx_t p = 2;                     /* current candidate prime */

    while (found < target) {
        if (is_prime(p)) {           /* p itself is prime */
            if (is_prime(p + 10)) {  /* check if p+10 is prime */
                ++found;
                if (found == target) {
                    return (seq_t)(p + 10);
                }
            }
        }
        /* advance to next candidate: 3 then odd numbers only */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }

    /* Should never reach here */
    return 0;
}
