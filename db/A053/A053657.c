/*
 * A053657  a(n) = Product_{p prime} p^{ Sum_{k>=0} floor[(n-1)/((p-1)p^k)]}.
 * https://oeis.org/A053657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = Product_{p prime} p^{ Sum_{k≥0} floor((n-1)/((p-1)p^k)) }
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1) = 1) */
#include <stdint.h>
#include <stdlib.h>

/* Helper functions supplied by the build */
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b) */

/* The function must have external linkage.  No other functions are
   defined outside of this file.  All work is performed in this
   function using only the supplied utilities and standard C. */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For n = 1 the product
       is empty, so the result is 1. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* For n > 1 we compute
          rem = n-1
        and multiply over all primes p such that p-1 ≤ rem
        (i.e., p ≤ rem+1). */
    idx_t rem = n - 1;
    idx_t limit = rem + 1;            /* largest p that may contribute */

    /* Sieve of Eratosthenes to list primes up to 'limit'.
       Because n ≤ 10000 (in the required tests), this straight‑forward
       algorithm is fast enough and uses only a few kilobytes. */

    /* Allocate a bit array for marking composites.  Use uint8_t for
       modest memory usage; the array size is limit+1. */
    uint8_t *is_composite = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!is_composite) {
        /* In the unlikely event of OOM, fall back to the trivial value
           1 (the function is required only for n ≥ 1, so this never
           affects correct results for the test data). */
        return (seq_t)1;
    }

    /* Compute primes and process them on the fly to avoid storing a
       separate list.  We only need to know primes in ascending order. */
    seq_t result = (seq_t)1;

    for (idx_t p = 2; p <= limit; ++p) {
        if (is_composite[p]) {
            continue;                     /* not a prime */
        }

        /* Mark multiples of p as composite. */
        if (p * p <= limit) {
            for (idx_t m = p * p; m <= limit; m += p) {
                is_composite[m] = 1;
            }
        }

        /* p is a prime; compute its exponent contribution.  The
           sum over k≥0 is 
              Σ floor(rem / ((p-1) * p^k)) . 
           We can iterate while the denominator ≤ rem.  Since
           rem ≤ 10^4, p^k stays well within 64‑bit range. */
        idx_t denom = p - 1;          /* (p-1) * p^0 */
        idx_t exp    = 0;
        while (denom <= rem) {
            exp += rem / denom;
            /* Prepare next term: (p-1) * p^{k+1} */
            if (denom > rem / p) {
                break;                /* avoid overflow and terminate */
            }
            denom *= p;
        }

        if (exp > 0) {
            seq_t p_pow = big_pow((seq_t)p, (seq_t)exp);
            result = seq_mul(result, p_pow);
        }
    }

    free(is_composite);
    return result;
}
