/*
 * A134034  Number of primes between n^K and (n+1)^K, where K is
 *          1.647862393566467... and n is an integer greater than 0.
 * https://oeis.org/A134034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p : n^K < p < (n+1)^K } with K =
 *            1.647862393566467
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static state for the sieve */
static uint8_t  *is_comp   = NULL;   /* composite flag array  */
static uint32_t *psum      = NULL;   /* prefix prime counts   */
static uint32_t *primes    = NULL;   /* prime list for sieve  */
static size_t    primes_cnt = 0;
static size_t    sieve_limit = 0;

/* Rebuild the sieve up to at least 'limit' */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit)
        return;

    free(is_comp);
    free(psum);
    free(primes);

    is_comp = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    psum    = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));

    size_t primes_cap = 1024;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    primes_cnt = 0;

    /* Linear sieve */
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt && i * (size_t)primes[j] <= limit; ++j) {
            is_comp[i * (size_t)primes[j]] = 1;
            if (i % primes[j] == 0)
                break;
        }
    }

    /* Build prefix counts */
    psum[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        psum[i] = psum[i-1] + (!is_comp[i]);

    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1, but guard anyway */
        return (seq_t)0;

    const long double K = 1.647862393566467L;

    /* Compute bounds using long double for precision */
    long double low  = powl((long double)n, K);
    long double high = powl((long double)n + 1.0L, K);

    /* Convert to integer indices */
    idx_t lower = (idx_t)floorl(low) + 1;          /* smallest integer > n^K   */
    idx_t upper = (idx_t)floorl(high);            /* largest integer < (n+1)^K */

    if (lower > upper)
        return (seq_t)0;

    ensure_sieve((size_t)upper);

    /* Count primes in [lower, upper] */
    uint32_t count = psum[upper] - psum[lower - 1];

    return (seq_t)count;
}
