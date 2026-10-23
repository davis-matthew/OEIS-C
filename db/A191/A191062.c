/*
 * A191062  Primes p that have Kronecker symbol (p|15) = -1.
 * https://oeis.org/A191062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p for which the Kronecker symbol (p|15) = -1,
   i.e. primes congruent to 7, 11, 13 or 14 modulo 15.  The sequence is 1‑based,
   so a(1) = 7. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_sieve.h"

seq_t a(idx_t n)
{
    if (n == 0) {  /* undefined at offset 0; return 0 as a sentinel */
        return (seq_t)0;
    }

    /* Initial bound.  Roughly we need about n*15 candidates because only 4/15 of
       the primes qualify; a multiplier of 10 works for the intended limits.  */
    idx_t limit = n * 10 + 100;

    while (1) {
        /* Allocate the sieve array.  Cast the void* from calloc to bool* to be
           C++-safe. */
        bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
        if (!is_comp) {
            /* allocation failure – propagate 0 */
            return (seq_t)0;
        }

        /* Sieve of Eratosthenes */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (!is_comp[i]) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_comp[j] = true;
                }
            }
        }

        /* Count primes of the desired residue classes */
        idx_t count = 0;
        idx_t candidate_prime = 0;

        for (idx_t p = 2; p <= limit; ++p) {
            if (is_comp[p]) {
                continue;           /* not prime */
            }
            /* Check the residue modulo 15 */
            uint64_t r = p % 15;
            if (r == 7 || r == 11 || r == 13 || r == 14) {
                ++count;
                if (count == n) {
                    candidate_prime = p;
                    break;
                }
            }
        }

        free(is_comp);

        if (count >= n) {
            /* cast to seq_t – this works whether seq_t is an unsigned integer
               type or an arbitrary‑precision class that accepts a unsigned long long. */
            return (seq_t)candidate_prime;
        }

        /* Not enough primes found – increase the bound and try again. */
        limit = limit * 2 + 200;
    }
}
