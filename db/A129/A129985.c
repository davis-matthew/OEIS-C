/*
 * A129985  Multiplicative persistence of the prime numbers.
 * https://oeis.org/A129985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = multiplicative persistence of the nth prime
 * Range:     u8 n <= 54, u16 n <= 7141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/digit_product.h"

static idx_t *prime_cache = NULL;   /* array of the first primes */
static size_t prime_cnt = 0;        /* number of primes stored */

/* Ensure that prime_cache contains at least the first n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    idx_t upper;
    if (n < 6)
        upper = 15;
    else {
        double dn = (double)n;
        upper = (idx_t)(dn * (log(dn) + log(log(dn))));
    }
    upper += 10; /* safety margin */

    if (prime_cache)
        free(prime_cache);
    build_primes(upper, &prime_cache, &prime_cnt);
}

/* Compute the multiplicative persistence of a value. */
static seq_t persistence(seq_t x)
{
    seq_t steps = 0;
    while (x >= 10) {
        x = digit_product(x);
        steps += 1;
    }
    return steps;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* out of bounds; not used in OEIS tests */

    ensure_primes(n);
    uint64_t p = prime_cache[n - 1];
    return persistence((seq_t)p);
}
