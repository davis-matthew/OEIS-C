/*
 * A167464  Primes p such that 20*p+19 is composite.
 * https://oeis.org/A167464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: return the n-th prime p such that 20*p+19 is composite. */
/* The function searches primes on demand using the helper is_prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* The sequence is indexed from 1.  For n == 0 we return 0 as an error. */
    if (n == 0)
        return 0;

    idx_t count = 0;      /* number of qualifying primes found so far */
    idx_t cand   = 2;     /* candidate prime to test */

    for (;;) {
        /* Test primality of the candidate. */
        if (is_prime(cand)) {
            /* Compute 20*cand+19 and test if it is composite. */
            idx_t comp = 20 * cand + 19;
            if (!is_prime(comp)) {
                ++count;
                if (count == n)
                    return (seq_t)cand;  /* found the desired term */
            }
        }

        /* Advance to the next candidate.  Skip even numbers after 2. */
        if (cand == 2)
            cand = 3;
        else
            cand += 2;
    }
}
