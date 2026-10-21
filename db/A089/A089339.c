/*
 * A089339  Number of primes beginning with n and of the form n concatenated
 *          with p where p takes all the prime values less than n.
 * https://oeis.org/A089339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 101, u32 n <= 101, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   build_primes_upto, concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A089339: a(n) = number of primes of the form concat(n, p)
 * where p runs over all primes less than n.
 * The implementation builds all primes < n and checks each concatenated
 * number for primality.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1U) return 0;                /* offset is 1, but guard for safety */

    uint64_t *primes = NULL;              /* array of primes < n */
    size_t      count_primes = 0;        /* number of primes in the array */

    /* Get primes less than n (use n-1 as inclusive upper bound) */
    build_primes_upto(n - 1U, &primes, &count_primes);

    seq_t result = 0;

    for (size_t i = 0; i < count_primes; ++i) {
        idx_t p = (idx_t)primes[i];
        seq_t candidate = concat_parts(n, p);   /* concatenate decimal strings */
        if (is_prime_big(candidate))
            ++result;
    }

    free(primes);                           /* clean up allocated memory */
    return result;
}
