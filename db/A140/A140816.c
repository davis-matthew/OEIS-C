/*
 * A140816  A third of digital roots of Bernoulli number denominators.
 * https://oeis.org/A140816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((digital_root(denominator(B_{2n})) + 2) / 3)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator of B_k (k even) is the product of all primes p such that
   p‑1 divides k.  The digital root is computed modulo 9, reducing the
   calculation to a simple sieve up to k+1.  The implementation works for
   any index n (idx_t) and returns the result in seq_t. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* k = 2*n, the index of the Bernoulli number whose denominator
       we need.  For n=0 we have k=0 and the denominator is 1. */
    idx_t k = 2*n;

    /* Sieve all primes <= k+1.  For n up to about 10000 this is tiny. */
    uint64_t limit = k + 1;                 /* inclusive upper bound */
    unsigned char *is_prime = (unsigned char*)calloc(limit + 1, 1);
    if (!is_prime)                         /* allocation failure */
        return (seq_t)0;

    for (uint64_t i = 2; i <= limit; ++i)  /* initially mark everything as prime */
        is_prime[i] = 1;

    for (uint64_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (uint64_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    /* Compute the product of all primes p with (p-1) | k,
       but only modulo 9 since we need the digital root. */
    uint32_t prod_mod9 = 1;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (is_prime[p] && k % (p - 1) == 0) {
            prod_mod9 = (prod_mod9 * (uint32_t)(p % 9)) % 9;
        }
    }

    free(is_prime);

    uint32_t dr = prod_mod9;          /* digital root of the denominator */
    if (dr == 0)                     /* never occurs but harmless */
        dr = 9;

    /* Convert the digital root to the sequence value. */
    return (seq_t)((dr + 2) / 3);
}
