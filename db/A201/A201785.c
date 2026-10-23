/*
 * A201785  Primes of the form 5n^2 - 3.
 * https://oeis.org/A201785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 5*k^2 - 3
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists these primes in increasing order of k (the index) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but harmless */

    idx_t count = 0;      /* how many primes found so far */
    idx_t k = 1;          /* current k to test */

    while (1) {
        /* Compute 5*k^2 - 3 using seq_t arithmetic to avoid overflow of k */
        seq_t val = (seq_t)5 * k * k - 3;

        /* The product 5*k^2 may exceed 64 bits for very large k.  
           In the test ranges it safely fits in uint64_t, so we cast. */
        if (val <= (seq_t)UINT64_MAX && is_prime((uint64_t)val)) {
            count++;
            if (count == n) {
                return val;
            }
        }
        k++;
    }
}
