/*
 * A031385  a(n) = prime(4*n-3).
 * https://oeis.org/A031385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(4*n-3)
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 991 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

static uint64_t *prime_array = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */
static idx_t    sieve_limit  = 0;     /* last limit used for sieving */

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n = 0 is undefined but we protect it. */
    if (n == 0)
        return (seq_t)0;

    /* Desired index in the prime list: k = 4*n - 3. */
    idx_t k = 4 * n - 3;

    /* If we don't have enough primes, generate a larger sieve. */
    if (prime_count < (size_t)k)
    {
        idx_t new_limit = _prime_limit_for_idx(k);

        if (new_limit > sieve_limit)
        {
            /* Free previous sieve buffer. */
            if (prime_array)
                free(prime_array);

            build_primes_upto(new_limit, &prime_array, &prime_count);
            sieve_limit = new_limit;
        }
    }

    /* k is 1‑based; array indices are 0‑based. */
    return (seq_t)prime_array[k - 1];
}
