/*
 * A061149  Smallest number whose number of divisors = n-th primorial
 *          (A002110).
 * https://oeis.org/A061149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = Product_{j=1..n} prime(j)^(prime(n-j+1)-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   build_primes, pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 (empty product)                                   */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers provided by the build environment */
#include "../utils/build_primes.h"
#include "../utils/pow_seq_t.h"

/* Cache for the first primes.  prime_array[k] = the (k+1)-th prime  */
static idx_t *prime_array = NULL;
static size_t   prime_cnt  = 0;

/* Grow the prime cache until it contains at least “needed” primes.   */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;

    /* Rough estimate of an upper bound that should contain the
       requested number of primes.  A multiple of 20 is safe.      */
    idx_t limit = (needed < 10) ? 100 : needed * 20;

    /* Build the prime list once and keep it for future calls.    */
    if (prime_array != NULL)
        free(prime_array);

    build_primes(limit, &prime_array, &prime_cnt);

    /* If we still do not have enough primes, double the limit and rebuild. */
    while (prime_cnt < (size_t)needed) {
        limit *= 2;
        if (prime_array != NULL)
            free(prime_array);
        build_primes(limit, &prime_array, &prime_cnt);
    }
}

/* Main sequence function – external linkage as required.          */
seq_t a(idx_t n)
{
    /* a(0) is the empty product. */
    if (n == 0)
        return (seq_t)1;

    /* We need the first n primes. */
    ensure_primes(n);

    seq_t result = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t base  = prime_array[j - 1];       /* prime(j)                 */
        idx_t exp   = prime_array[n - j] - 1;   /* prime(n-j+1)-1           */
        result *= pow_seq_t(base, exp);         /* multiply into the product */
    }
    return result;
}
