/*
 * A214567  Maximal number of distinct rooted trees obtained from the rooted
 *          tree with Matula-Goebel number n by adding one pendant edge at one
 *          of its vertices.
 * https://oeis.org/A214567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; if n is prime with index t then a(n) = 1 + a(t); if n
 *            is composite then a(n) = 1 + Σ_{p|n}(a(p)−1), summed over
 *            distinct prime divisors p of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic cache for a(n).  cache[0] is unused. */
static seq_t *a_cache = NULL;
static idx_t cache_cap = 0;

/* prime_index[i] = 0 if i not prime, else its 1‑based index among primes. */
static idx_t *prime_index = NULL;
/* list of primes in ascending order. */
static idx_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_cnt = 0;

/* Ensure that a_cache[1..n] and primes list are computed up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;          /* already computed */

    /* Grow cache arrays to size n+1 (index 0 unused). */
    size_t new_cap = (size_t)n + 1;
    a_cache      = (seq_t*)realloc(a_cache,      new_cap * sizeof(seq_t));
    prime_index  = (idx_t*)realloc(prime_index,  new_cap * sizeof(idx_t));
    if (!a_cache || !prime_index) {
        /* allocation failed; terminate. */
        exit(EXIT_FAILURE);
    }

    /* Allocate primes array if needed. */
    if (!primes) {
        primes_cap = 16;
        primes = (idx_t*)malloc(primes_cap * sizeof(idx_t));
        if (!primes) exit(EXIT_FAILURE);
    }

    /* Compute missing entries. */
    for (idx_t i = cache_cap + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[1] = (seq_t)1;
            prime_index[1] = 0;
            continue;
        }

        /* Test primality using known primes. */
        bool is_prime = true;
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p * p > i) break;
            if (i % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* i is prime. t = its 1‑based index among primes. */
            idx_t t = (idx_t)primes_cnt + 1;
            a_cache[i] = a_cache[t] + (seq_t)1;

            /* Record new prime. */
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = i;
            prime_index[i] = t;
        } else {
            /* i is composite: sum over distinct prime divisors. */
            seq_t sum = 0;
            idx_t tmp = i;
            for (size_t j = 0; j < primes_cnt; ++j) {
                idx_t p = primes[j];
                if (p * p > tmp) break;
                if (tmp % p == 0) {
                    sum += a_cache[p] - (seq_t)1;
                    while (tmp % p == 0) tmp /= p;
                }
            }
            if (tmp > 1) { /* remaining prime factor */
                sum += a_cache[tmp] - (seq_t)1;
            }
            a_cache[i] = sum + (seq_t)1;
            prime_index[i] = 0;
        }
    }

    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n < 1 */
    ensure_cache(n);
    return a_cache[n];
}
