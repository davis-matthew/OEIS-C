/*
 * A086937  Number of distinct zeros of x^2-x-1 mod prime(n).
 * https://oeis.org/A086937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if prime(n)=5, 2 if LegendreSymbol(5,prime(n))=1,
 *            otherwise 0. Computes the number of distinct zeros of x²‑x‑1 in
 *            GF(p) where p is the n-th prime
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global storage for primes and a trivial sieve. */
static uint8_t *sieve   = NULL;   /* 0 = prime, 1 = composite, indexed by integer value */
static idx_t   sieve_limit = 0;   /* maximum integer currently sieved */
static uint64_t *prime_list = NULL;  /* array of primes found */
static idx_t   prime_count  = 0;     /* number of primes stored */

/* Ensure that at least 'n' primes are available in prime_list. */
static void ensure_n_primes(idx_t n)
{
    if (n <= prime_count) return;  /* already enough */

    /* Rough upper bound for the n-th prime (Rosser's theorem says p_n < n(log n + log log n), but
       for n ≤ 10000 20n + 20 is more than enough). */
    idx_t limit = n * 20 + 20;
    if (limit < 20) limit = 20;

    /* Allocate or resize the sieve memory. */
    if (sieve) free(sieve);
    sieve = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!sieve) return;  /* allocation failed – unlikely for these sizes */

    /* Eratosthenes sieve.  sieve[i]==0 means prime. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Count primes up to 'limit'. */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;

    /* Allocate prime list. */
    if (prime_list) free(prime_list);
    prime_list = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
    if (!prime_list) return;  /* allocation failed – unlikely */

    /* Fill prime list. */
    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            prime_list[pos++] = i;
        }
    }

    prime_count = cnt;
    sieve_limit = limit;
}

/* Binary exponentiation modulo 'mod' using 128‑bit multiplication
   to avoid overflow; the result fits in uint64_t because mod ≤ 2^64‑1. */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined below offset, return 0 */

    ensure_n_primes(n);      /* guarantees prime_list[n-1] is valid */

    uint64_t p = prime_list[n - 1];
    if (p == 5ULL) return 1;          /* double root: one distinct zero */
    if (p == 2ULL || p == 3ULL) return 0;  /* no solutions */

    /* Compute Legendre symbol (5|p) via Euler's criterion. */
    uint64_t leg = powmod64(5ULL, (p - 1) / 2, p);
    if (leg == 1) return 2;   /* two distinct roots */
    return 0;                 /* none */
}
