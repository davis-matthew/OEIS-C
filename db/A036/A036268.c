/*
 * A036268  7th differences of primes.
 * https://oeis.org/A036268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=0}^{7} coeffs[i] * p_{n+i} with coeffs = { -1, 7,
 *            -21, 35, -35, 21, -7, 1 } (p_k denotes the k‑th prime, with p_1
 *            = 2)
 * Range:     i8 n <= 26, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for primes */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;   /* number of primes stored */
static idx_t     sieve_limit = 0;   /* last sieve limit used */

/* Sieve of Eratosthenes up to 'limit', storing primes in prime_list. */
static void sieve_primes(idx_t limit)
{
    /* Allocate boolean array. */
    unsigned char *is_prime;
    is_prime = (unsigned char *)malloc((size_t)(limit + 1) * sizeof(unsigned char));
    if (!is_prime)
        return;

    /* Initialise */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* Allocate array for primes */
    uint64_t *list = NULL;
    list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!list) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i])
            list[idx++] = (uint64_t)i;
    }

    free(is_prime);
    free(prime_list);
    prime_list  = list;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Ensure that at least 'need' primes are available.  The sieve limit
   is doubled until enough primes have been gathered. */
static void ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    idx_t lim = (sieve_limit > 0) ? sieve_limit : 1000;
    while (prime_count < (size_t)need) {
        if (lim < 2)
            lim = 1000;
        sieve_primes(lim);
        if (prime_count >= (size_t)need)
            break;
        lim *= 2;            /* expand for next attempt */
    }
}

/* Binomial coefficients for Δ⁷:  C(7,i) with alternating signs. */
static const int coeffs[8] = {
    -1, 7, -21, 35, -35, 21, -7, 1
};

/* ------------------------------------------------------------------ */
/*  OEIS sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; below undefined */

    /* Need primes up to index n+7 (1‑based).  The nth prime is at index n‑1. */
    ensure_primes(n + 7);

    seq_t result = 0;
    for (int i = 0; i < 8; ++i)
        result += (seq_t)coeffs[i] * (seq_t)prime_list[n - 1 + i];

    return result;
}
