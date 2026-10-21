/*
 * A097059  Numbers of the form p^3 + 3^p for p prime.
 * https://oeis.org/A097059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n^3 + 3^{p_n} (p_n is the n-th prime)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/ipow.h"

/* Static cache for primes (up to about 200 000, enough for n≈10 000) */
static uint32_t primes[200000];
static size_t prime_cnt = 0;
static idx_t max_n_seen = 0;

/* Estimate an upper bound for the n-th prime (Rosser's theorem) */
static idx_t estimate_upper_prime(idx_t n)
{
    if (n < 6)
        return 15;
    double dn = (double)n;
    double bound = dn * (log(dn) + log(log(dn)));
    return (idx_t)(bound + 3.0);           /* add small safety margin */
}

/* Generate all primes up to 'limit' and fill the static cache */
static void generate_primes_up_to(idx_t limit)
{
    uint8_t *sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!sieve)
        return; /* allocation failure: leave cache empty */

    /* Initialize sieve */
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (idx_t i = 2; (i * i) <= limit; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    /* Collect primes into cache */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit && prime_cnt < sizeof(primes)/sizeof(primes[0]); ++i)
        if (sieve[i])
            primes[prime_cnt++] = (uint32_t)i;

    free(sieve);
}

/* Ensure that the cache contains at least 'n' primes */
static void ensure_primes(idx_t n)
{
    if (n <= max_n_seen)
        return;   /* cache already sufficient */

    /* Compute new upper bound and generate primes */
    idx_t new_limit = estimate_upper_prime(n) * 2; /* extra safety */
    generate_primes_up_to(new_limit);

    max_n_seen = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, so a(0) is undefined – return 0 for safety */
        return 0;

    ensure_primes(n);

    uint32_t p = primes[n - 1];          /* the n-th prime */
    seq_t p_seq = (seq_t)p;

    seq_t p_cubed = ipow(p_seq, 3);
    seq_t pow3_p  = ipow((seq_t)3, p);

    return p_cubed + pow3_p;
}
