/*
 * A072548  a(n) = sigma(n) mod PrimePi(n).
 * https://oeis.org/A072548
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) % pi(n) (sum of divisors modulo prime-count)
 * Range:     u8 n <= 354, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/* Local cached prime data                                           */
/*------------------------------------------------------------------*/
static size_t sieve_limit = 0;          /* largest n for which sieve is built           */
static uint8_t *prime_flags  = NULL;    /* 1 = prime, 0 = composite (indices 0..sieve_limit) */
static unsigned int *pi_cache = NULL;   /* pi_cache[i] = number of primes ≤ i           */

/* Build or extend the prime sieve up to n (inclusive).            */
static void build_sieve(idx_t n)
{
    if (n <= sieve_limit) return;                 /* already sufficient */

    free(prime_flags);
    free(pi_cache);

    sieve_limit = (size_t)n;
    size_t size = sieve_limit + 1;

    /* Allocate and initialize all entries to prime (1) except 0,1 */
    prime_flags = (uint8_t *)malloc(size * sizeof(uint8_t));
    if (!prime_flags) return;
    for (size_t i = 0; i < size; ++i) prime_flags[i] = 1;
    if (size > 0) prime_flags[0] = 0;
    if (size > 1) prime_flags[1] = 0;

    /* Eratosthenes sieve */
    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (prime_flags[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i)
                prime_flags[j] = 0;
        }
    }

    /* Build pi(n) cache */
    pi_cache = (unsigned int *)malloc(size * sizeof(unsigned int));
    if (!pi_cache) return;
    unsigned int count = 0;
    for (size_t i = 0; i < size; ++i) {
        if (i >= 2 && prime_flags[i]) ++count;
        pi_cache[i] = count;
    }
}

/* Compute sigma(n) = sum of all positive divisors of n.          */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += i;
            idx_t j = n / i;
            if (j != i) sum += j;
        }
    }
    return sum;
}

/*------------------------------------------------------------------*/
/* Public sequence function                                         */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n ≥ 2 by specification. */
    build_sieve(n);

    seq_t sigma = sigma_of(n);
    seq_t pi    = (seq_t)pi_cache[n];   /* pi(n) cannot be zero for n ≥ 2 */

    return sigma % pi;
}
