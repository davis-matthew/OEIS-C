/*
 * A094300  Duplicate of A055773.
 * https://oeis.org/A094300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = product of all primes p with n/2 < p <= n
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    /* enlarge the prime table if necessary */
    if (n > prime_limit)
    {
        if (primes)
            free(primes);               /* free old table */
        build_primes_upto(n, &primes, &prime_count);
        prime_limit = n;
    }

    /* threshold p > n/2  →  smallest integer >= n/2 + 1 */
    idx_t half = n / 2 + 1;

    seq_t prod = 1;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = primes[i];
        if (p < half)
            continue;                 /* p is too small */
        if (p > n)
            break;                    /* beyond the interval */
        prod *= (seq_t)p;            /* multiply into result */
    }
    return prod;
}
