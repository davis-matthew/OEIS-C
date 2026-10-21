/*
 * A235398  Sum of digits of the cubes of prime numbers.
 * https://oeis.org/A235398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A030078(n))
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the digit sum of the cube of the n-th prime (offset 1). */

#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes.h"
#include "../utils/digit_sum.h"

/* Cache of primes (1‑based indexing). */
static idx_t *prime_cache = NULL;   /* array of primes */
static size_t prime_count = 0;      /* number of primes stored */

/* Ensure that prime_cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_cache && prime_count >= n)
        return;

    /* Rough upper bound: for n >= 6, p_n < n*(log n + log log n) < 15*n for n <= 10^6. */
    idx_t limit = (n > 5) ? (n * 15 + 100) : 200u;

    while (1) {
        size_t cnt;
        idx_t *tmp;

        build_primes(limit, &tmp, &cnt);

        /* free old cache if any */
        if (prime_cache)
            free(prime_cache);

        prime_cache = tmp;
        prime_count = cnt;

        if (prime_count >= n)
            break;

        /* increase the limit and try again */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;  /* undefined for indices < 1 */

    /* Ensure we have the needed prime. */
    ensure_primes(n);

    /* prime(n) is at prime_cache[n-1] (array is 0‑based). */
    idx_t p = prime_cache[n - 1];

    /* Compute p^3 in seq_t. */
    seq_t cube = (seq_t)p;
    cube = cube * cube;
    cube = cube * p;

    /* Return the digit sum. */
    return digit_sum(cube);
}
