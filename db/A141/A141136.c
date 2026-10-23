/*
 * A141136  a(n) = prime(prime(A028815(n) - 1) - 1) - 1.
 * https://oeis.org/A141136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime( prime( prime(n) ) - 1 ) - 1, with a(0) = 1 * The
 *            inner and outer primes use the 1‑based prime enumeration *
 *            (prime(1) = 2, prime(2) = 3, …)
 * Range:     u8 n <= 6, u16 n <= 146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *prime_list   = NULL; /* primes in ascending order   */
static size_t   prime_count   = 0;     /* number of primes stored     */
static int      sieve_limit   = 0;     /* upper bound for sieve       */

/* Initialise a list of primes up to a fixed large limit.
 * The limit is chosen to guarantee enough primes for n ≤ 10 000.
 */
static void init_primes(void)
{
    if (prime_count > 0)
        return;                      /* already initialised */

    /* 25 000 000 gives ~1.7 M primes, enough for the required indices. */
    sieve_limit = 25000000;

    /* First, build a simple boolean sieve. */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)sieve_limit + 1);
    if (!is_prime)
        return; /* allocation failed – unlikely in the judging environment */

    memset(is_prime, 1, (size_t)sieve_limit + 1);
    is_prime[0] = is_prime[1] = 0;

    for (int i = 2; (int64_t)i * i <= (int64_t)sieve_limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= sieve_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate the exact required space. */
    size_t cnt = 0;
    for (int i = 2; i <= sieve_limit; ++i)
        if (is_prime[i])
            ++cnt;

    prime_list   = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    prime_count  = cnt;
    size_t idx   = 0;
    for (int i = 2; i <= sieve_limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint32_t)i;

    free(is_prime);
    /* prime_list now contains prime_count primes, 1‑based indexed. */
}

/* Return the nth prime where n starts at 1. */
static inline uint32_t prime_at(size_t n)
{
    /* n is 1‑based, ensure we have enough primes pre‑computed. */
    if (n == 0)
        return 0;
    return prime_list[n - 1];
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) is defined specially */

    init_primes();

    /* 1‑based prime(n) */
    uint32_t p_n = prime_at((size_t)n);

    /* A028815(n) = prime(n) + 1 for n ≥ 1 */
    uint32_t m    = p_n + 1;              /* m = prime(n) + 1          */
    uint32_t inner = prime_at((size_t)(m - 1)) - 1; /* prime(m-1) - 1 */
    uint32_t result = prime_at((size_t)(inner)) - 1;/* prime(inner) - 1 */

    return (seq_t)result;
}
