/*
 * A240668  Number of the first odd exponents in the prime power factorization
 *          of (2*n)!.
 * https://oeis.org/A240668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: counts the number of initial consecutive odd exponents
   in the prime factorisation of (2*n)!  (OEIS A240668).  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Safety: the sequence is defined for n >= 1
       (the build system never calls with n = 0). */
    if (n == 0) return 0;

    idx_t limit = 2 * n;              /* we need primes up to 2*n */
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) return 0;           /* out‑of‑memory: return 0 */

    seq_t count = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {            /* p is prime */
            seq_t exp = 0;
            idx_t power = p;
            while (power <= limit) {
                exp += limit / power;
                if (power > limit / p) break;   /* avoid overflow */
                power *= p;
            }
            if (exp & 1) {            /* odd exponent */
                count++;
            } else {                  /* first even exponent -> stop */
                free(is_comp);
                return count;
            }

            /* Mark composite multiples of p for the sieve */
            if (p <= limit / p) {
                for (idx_t mult = p * p; mult <= limit; mult += p)
                    is_comp[mult] = true;
            }
        }
    }

    free(is_comp);
    return count;
}
