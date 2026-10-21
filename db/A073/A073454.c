/*
 * A073454  Number of repeated remainders arising when n is divided by all
 *          primes up to n: a(n) = pi(n) - A073453(n).
 * https://oeis.org/A073454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A073454 – number of repeated remainders when n is divided by all primes ≤ n.
   Formula:  a(n) = π(n) – A073453(n)   (π(n) = number of primes ≤ n,  
   A073453(n) = number of distinct remainders (n mod p) for all p ≤ n). 
   This implementation computes the number of duplicated remainders directly. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; below that value is undefined */

    /* build the list of primes ≤ n */
    size_t prime_cnt;
    uint64_t *primes;
    build_primes_upto(n, &primes, &prime_cnt);

    /* keep track of seen remainders.  The remainder r is < p ≤ n,
       hence 0 ≤ r < n; array size n+1 is enough. */
    bool *seen = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!seen) {           /* memory allocation failed – return 0 as safe fallback */
        free(primes);
        return 0;
    }

    seq_t repeated = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t r = n % p;
        if (seen[r])          /* remainder already observed → duplicate */
            ++repeated;
        else
            seen[r] = true;
    }

    free(seen);
    free(primes);          /* build_primes_upto uses malloc */
    return repeated;
}
