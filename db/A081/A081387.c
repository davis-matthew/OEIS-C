/*
 * A081387  Number of non-unitary prime divisors of central binomial
 *          coefficient, C(2n,n) = A000984(n), i.e., number of prime factors
 *          in C(2n,n) whose exponent is greater than one.
 * https://oeis.org/A081387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { primes p | v_p( C(2n,n) ) > 1 } (non‑unitary prime
 *            divisors)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Static sieve state */
static uint32_t *primes = NULL;          /* array of primes ≤ sieve_limit */
static size_t   prime_cnt   = 0;         /* number of stored primes        */
static idx_t    sieve_limit = 0;         /* maximum n that has been sieved */

/* Ensure prime list up to at least 'limit' (limit = 2*n) */
static bool ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)            /* already available */
        return true;

    /* Allocate temporary sieve */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime)
        return false;

    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }

    /* Count primes and allocate array */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    uint32_t *new_primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!new_primes) {
        free(is_prime);
        return false;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_primes[idx++] = (uint32_t)i;

    /* Replace old prime list */
    free(primes);
    primes     = new_primes;
    prime_cnt  = count;
    sieve_limit = limit;

    free(is_prime);
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0U;          /* below offset */

    /* We need primes up to 2*n */
    idx_t limit = n * 2U;
    if (!ensure_primes(limit))
        return 0U;                  /* allocation failure fallback */

    seq_t cnt = 0U;                 /* result */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        if ((idx_t)p > limit)
            break;

        /* Legendre valuation of C(2n,n) for prime p */
        idx_t vp = 0;
        idx_t pow = p;
        while (pow <= limit) {
            idx_t term = (limit / pow) - 2U * (n / pow);
            vp += term;
            if (pow > limit / p)        /* prevent overflow of pow*=p */
                break;
            pow *= p;
        }
        if (vp > 1)
            ++cnt;                      /* count non‑unitary prime divisors */
    }
    return cnt;
}
