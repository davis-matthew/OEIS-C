/*
 * A214064  a(n) is the least m > 0 such that Fibonacci(n+m) and n-m are not
 *          relatively prime.
 * https://oeis.org/A214064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m > 0 such that gcd(F(n+m), n-m) > 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static cache of small primes for factorisation. */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_initialized = false;
#define PRIME_SIEVE_LIMIT 50000

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    /* Allocate enough space for all primes up to PRIME_SIEVE_LIMIT. */
    prime_list = (uint32_t*)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(uint32_t));
    /* Basic sieve. */
    bool *sieve = (bool*)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    memset(sieve, false, (PRIME_SIEVE_LIMIT + 1) * sizeof(bool));

    prime_count = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
}

/* Fast doubling to compute Fibonacci numbers modulo mod. */
static void fib_pair_mod(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;   /* not needed but keeps the interface valid */
        return;
    }
    uint64_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);
    uint64_t two_b  = (b << 1) % mod;
    uint64_t c = (a * ((two_b + mod - a) % mod)) % mod;               /* F(2k)  */
    uint64_t d = (a * a % mod + b * b % mod) % mod;                   /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

static uint64_t fib_mod(uint64_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    for (idx_t m = 1; ; ++m) {
        idx_t k  = (n > m) ? (n - m) : (m - n);   /* |n-m| */
        if (k == 0) {                             /* gcd with 0 => |F(2n)| */
            if (n > 1) return (seq_t)m;          /* F(2n) > 1 for n > 1 */
            continue;                            /* n==1 case handled below */
        }

        uint64_t absk = (uint64_t)k;
        if (absk <= 1u) continue;                /* no prime factors */

        /* collect distinct prime factors of absk */
        uint32_t factors[64];
        size_t fcnt = 0;
        uint64_t temp = absk;
        for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
            uint32_t p = prime_list[i];
            if (temp % p == 0) {
                factors[fcnt++] = p;
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) factors[fcnt++] = (uint32_t)temp;

        /* check divisibility of F(n+m) by any prime factor */
        uint64_t Fn_mod;
        for (size_t i = 0; i < fcnt; ++i) {
            uint32_t p = factors[i];
            Fn_mod = fib_mod((uint64_t)(n + m), (uint64_t)p);
            if (Fn_mod == 0) return (seq_t)m;
        }
    }
}
