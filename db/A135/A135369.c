/*
 * A135369  a(0)=1, a(1)=2; thereafter, let n! = p(1)^b(1)...p(r)^b(r) be the
 *          prime factorization of n!. Then a(n) = Sum_{i=1..r} (p(i) + b(i)).
 * https://oeis.org/A135369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2; for n>=2 a(n)= Sum_{p<=n} p + Sum_{p<=n}
 *            v_p(n!); Equivalent recurrence: for n>=3 a(n)=a(n-1) +
 *            (isprime(n)? n : 0) + Sum_{p|n} v_p(n)
 * Range:     u8 n <= 36, u16 n <= 906, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation caches partial values and pre‑computes a sieve
   of primes up to the largest n seen. The calculation is O(n log log n)
   overall and works for n up to about 10 000 in well under 2 s. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Parallel storage for the sequence; a cache that grows as needed. */
static seq_t *cache = NULL;          /* array of a(n) values */
static idx_t cache_len = 0;          /* number of initialized entries (cache_len == n+1)*/
static size_t cache_cap = 0;         /* allocated capacity in elements */

/* Sieve data */
static bool  *is_prime = NULL;       /* is_prime[i] tells if i is prime */
static size_t is_prime_cap = 0;      /* allocated capacity for is_prime */
static uint32_t *primes = NULL;      /* list of primes up to current limit */
static size_t primes_cnt = 0;        /* number of primes currently stored */
static idx_t prime_limit = 0;        /* largest n processed for sieve */

/* Ensure cache is large enough for the requested n, extending it if necessary. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;   /* already computed up to n */

    /* ----- Extend the sieve if needed ----- */
    if (prime_limit < n) {
        /* Allocate or extend is_prime array */
        size_t new_cap = n + 1;
        if (is_prime == NULL) {
            is_prime = (bool*)malloc(new_cap * sizeof(bool));
            is_prime_cap = new_cap;
        } else if (is_prime_cap < new_cap) {
            is_prime = (bool*)realloc(is_prime, new_cap * sizeof(bool));
            is_prime_cap = new_cap;
        }
        for (size_t i = 0; i <= n; ++i)
            is_prime[i] = true;
        if (new_cap > 0) is_prime[0] = false;
        if (new_cap > 1) is_prime[1] = false;

        /* Simple Eratosthenes sieve */
        for (size_t p = 2; p * p <= n; ++p) {
            if (is_prime[p]) {
                for (size_t m = p * p; m <= n; m += p)
                    is_prime[m] = false;
            }
        }

        /* Build prime list */
        if (primes == NULL) {
            /* Allocate a generous static array; for n <= 10000 < 1200 primes suffice. */
            primes = (uint32_t*)malloc(12000 * sizeof(uint32_t));
        }
        primes_cnt = 0;
        for (size_t i = 2; i <= n; ++i)
            if (is_prime[i])
                primes[primes_cnt++] = (uint32_t)i;

        prime_limit = n;
    }

    /* ----- Extend cache capacity ----- */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= n)
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* ----- Compute values from cache_len up to n ----- */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)3;
        } else {
            seq_t prev = cache[i - 1];
            seq_t add = 0;
            if (is_prime[i])
                add += (seq_t)i;

            /* Sum of prime exponents in i */
            idx_t j = i;
            seq_t sum_v = 0;
            for (size_t k = 0; k < primes_cnt && (idx_t)primes[k] * (idx_t)primes[k] <= j; ++k) {
                uint32_t p = primes[k];
                if (j % p == 0) {
                    int exp = 0;
                    do {
                        j /= p;
                        ++exp;
                    } while (j % p == 0);
                    sum_v += (seq_t)exp;
                }
            }
            if (j > 1)   /* remaining prime factor */
                sum_v += (seq_t)1;

            cache[i] = prev + add + sum_v;
        }
    }
    cache_len = n + 1;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
