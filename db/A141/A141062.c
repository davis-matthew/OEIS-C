/*
 * A141062  a(n) = (prime(n) - 1) mod (sum of digits of prime(n)).
 * https://oeis.org/A141062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A141062
 *  a(n) = (prime(n) – 1) mod (sum of digits of prime(n))
 *
 *  The function finds the n‑th prime, sums its decimal digits,
 *  and returns the remainder of (prime-1) modulo that sum.
 *  Primes are generated lazily and cached, so calls to a() may be
 *  in arbitrary order.
 */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Estimate an upper bound for the n-th prime (n >= 6) using Rosser's theorem.
 * For small n the constant 15 is sufficient. */
static size_t prime_upper_bound(idx_t n)
{
    if (n < 6u)
        return 15u;                   /* p5 = 11 */
    double dn = (double)n;
    double bound = dn * (log(dn) + log(log(dn))) + 3.0;
    return (size_t)bound;
}

/* Sum the decimal digits of a 64‑bit unsigned integer. */
static uint64_t digit_sum_uint64(uint64_t v)
{
    uint64_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Prime cache and helpers ---------------------------------------------- */
static uint64_t *prime_cache   = NULL;   /* array of primes                */
static size_t   prime_cnt     = 0;        /* number of primes cached        */
static size_t   prime_cap     = 0;        /* capacity of prime_cache        */
static size_t   sieve_limit   = 0;        /* last sieved limit              */

/* Ensure that prime_cache contains at least need primes. */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;                         /* already enough primes          */

    /* Extend the cache capacity if necessary. */
    while (prime_cap < (size_t)need) {
        prime_cap = prime_cap ? prime_cap * 2 : 256;
        prime_cache = (uint64_t*)realloc((void*)prime_cache,
                                         prime_cap * sizeof(uint64_t));
    }

    /* Decide on a suitable sieve limit. */
    size_t new_limit = sieve_limit ? sieve_limit : prime_upper_bound(need);
    if (new_limit < prime_upper_bound(need))
        new_limit = prime_upper_bound(need);

    /* Allocate a temporary sieve array. */
    size_t *marked = (size_t*)malloc((new_limit + 1) * sizeof(size_t));
    if (!marked) /* out of memory – caller will handle a zero result */
        return;

    /* Simple Sieve of Eratosthenes. */
    for (size_t i = 2; i <= new_limit; ++i)
        marked[i] = 0;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (!marked[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                marked[j] = 1;
        }
    }

    /* Fill the prime cache with primes found. */
    size_t idx = 0;
    for (size_t i = 2; i <= new_limit && idx < prime_cap; ++i) {
        if (!marked[i]) {
            prime_cache[idx++] = (uint64_t)i;
        }
    }
    prime_cnt = idx;
    sieve_limit = new_limit;
    free(marked);
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1u)               /* undefined for n == 0; caller will not use it */
        return 0;

    ensure_primes(n);          /* guarantees prime_cache[n-1] exists */
    uint64_t p = prime_cache[n - 1];          /* prime(n)   */
    uint64_t dsum = digit_sum_uint64(p);     /* sum of its digits */
    return (seq_t)((p - 1u) % dsum);          /* (p-1) mod dsum */
}
