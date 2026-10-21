/*
 * A081248  Number of numbers 1<=k<=n with mu(k+n)+mu(k)=0, where mu=A008683
 *          (Moebius function).
 * https://oeis.org/A081248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 1 ≤ k ≤ n : mobius(k + n) + mobius(k) = 0 }|
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Static storage for the Möbius sieve */
static int8_t   *mu = NULL;     /* Möbius values, indices 0..mu_max */
static uint8_t  *is_prime = NULL; /* prime flag array for linear sieve */
static uint32_t *primes = NULL;    /* primes array for the linear sieve */
static size_t   primes_len = 0;    /* number of primes currently stored */
static size_t   primes_cap = 0;    /* capacity of the primes array */
static idx_t    mu_max = 0;        /* largest n for which μ has been computed */

/* Ensure that μ(i) is available for all i ≤ limit (inclusive) */
static void
ensure_mu(idx_t limit)
{
    if (limit <= mu_max)
        return;

    /* Allocate/expand the μ and prime‑flag arrays */
    size_t size = limit + 1;          /* need indices 0..limit */
    int8_t  *new_mu = (int8_t *)realloc(mu, size * sizeof(int8_t));
    uint8_t *new_ip = (uint8_t *)realloc(is_prime, size * sizeof(uint8_t));
    if (!new_mu || !new_ip)
        abort();

    mu        = new_mu;
    is_prime  = new_ip;

    /* Guarantee prime storage capacity for the linear sieve */
    size_t new_cap = limit > 2000 ? limit : 2000; /* safety bound */
    uint32_t *new_pr = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!new_pr)
        abort();
    primes = new_pr;
    if (new_cap > primes_cap)
        primes_cap = new_cap;

    /* Initialise */
    mu[0] = 0;
    mu[1] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        mu[i] = 1;
        is_prime[i] = 1;
    }

    /* Linear sieve for μ and primes */
    primes_len = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            assert(primes_len < primes_cap);
            primes[primes_len++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            idx_t p = primes[j];
            idx_t mult = i * p;
            if (mult > limit)
                break;
            is_prime[mult] = 0;
            if (i % p == 0) {
                mu[mult] = 0;
                break;
            } else {
                mu[mult] = -mu[i];
            }
        }
    }

    mu_max = limit;
}

/* Sequence entry */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* the sequence is defined for n ≥ 1 */

    /* We need μ up to n + n (because of μ(k + n)) */
    ensure_mu(n + n);

    size_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (mu[k] == -mu[k + n])
            ++cnt;
    }
    return (seq_t)cnt;
}
