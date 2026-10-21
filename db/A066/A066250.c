/*
 * A066250  a(n) = if n+1 is prime then A049084(n+1)*2 - 1 else
 *          A066246(n+1)*2.
 * https://oeis.org/A066250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1 is prime) ? 2*π(n+1)-1 : 2*((n+1)-π(n+1)-1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* π(x) is the prime counting function. */

#include <stdlib.h>
#include <stdint.h>

/* The variables seq_t and idx_t are defined by the build environment */

/* Static sieve data */
static uint8_t  *prime_flags = NULL;   /* 1 if prime, 0 otherwise */
static uint32_t *pi_count    = NULL;   /* cumulative prime counts */
static size_t   sieve_max   = 0;        /* largest value already sieved */

/* Build a sieve up to 'limit' (inclusive) and compute π(x) */
static void build_sieve(size_t limit)
{
    /* free previous buffers if any */
    free(prime_flags);
    free(pi_count);

    prime_flags = (uint8_t *)malloc(limit + 1);
    pi_count    = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!prime_flags || !pi_count)
        return; /* allocation failed – unlikely for typical limits */

    /* initialise as all primes */
    for (size_t i = 0; i <= limit; ++i)
        prime_flags[i] = 1;
    prime_flags[0] = prime_flags[1] = 0;

    /* Eratosthenes sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_flags[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_flags[j] = 0;
        }
    }

    /* compute π(x) */
    pi_count[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        pi_count[i] = pi_count[i - 1] + prime_flags[i];

    sieve_max = limit;
}

/* get π(x); expands sieve if necessary */
static inline uint32_t get_pi(size_t x)
{
    if (x > sieve_max)
        build_sieve(x);
    return pi_count[x];
}

/* primality test; expands sieve if necessary */
static inline int is_prime(size_t x)
{
    if (x > sieve_max)
        build_sieve(x);
    return prime_flags[x];
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 0, but safe */

    size_t k = (size_t)(n + 1);
    uint32_t pi_k = get_pi(k);

    if (is_prime(k)) {
        /* a(n) = 2*π(n+1) - 1 */
        return (seq_t)pi_k * 2 - 1;
    } else {
        /* a(n) = 2*((n+1)-π(n+1)-1) */
        return ((seq_t)(k - pi_k - 1)) * 2;
    }
}
