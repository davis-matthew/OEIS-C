/*
 * A196062  Number of leaf-parents of the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A196062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; a(2)=1; if n=prime(t) (t‑th prime, t>1) then a(n)=a(t);
 *            if n=r·s (r,s≥2) and both r and s are even then
 *            a(n)=a(r)+a(s)-1; otherwise a(n)=a(r)+a(s)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global memoisation structures */
static seq_t *cache = NULL;
static unsigned char *computed = NULL;
static size_t cache_cap = 0;

/* Global prime infrastructure */
static bool *is_prime = NULL;
static size_t is_prime_cap = 0;
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static idx_t prime_limit = 0;

/* Ensure cache large enough for index n (1‑based). */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    computed = (unsigned char *)realloc(computed, new_cap * sizeof(unsigned char));
    if (!cache || !computed)
        exit(EXIT_FAILURE);
    for (size_t i = cache_cap; i < new_cap; ++i)
        computed[i] = 0;
    cache_cap = new_cap;
}

/* Re‑compute the sieve up to `limit` and fill the prime list. */
static void
ensure_primes(idx_t limit)
{
    if (limit == 0)
        return;
    size_t new_cap = (size_t)limit + 1;
    if (new_cap > is_prime_cap) {
        bool *new_arr = (bool *)realloc(is_prime, new_cap * sizeof(bool));
        if (!new_arr)
            exit(EXIT_FAILURE);
        is_prime = new_arr;
        is_prime_cap = new_cap;
    }
    for (size_t i = 2; i < new_cap; ++i)
        is_prime[i] = true;
    for (size_t i = 2; (uint64_t)i * (uint64_t)i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    /* rebuild prime list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt = 0;
    prime_cap = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;
        }
    }
    prime_limit = limit;
}

/* Smallest prime factor of n (assumes n >= 2). */
static seq_t
smallest_prime_factor(idx_t n)
{
    if (n % 2 == 0)
        return 2;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return (seq_t)p;
    }
    return (seq_t)n;  /* n is prime */
}

/* Recursive evaluation with memoisation. */
static seq_t
compute_recursive(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    if (computed[n])
        return cache[n];

    seq_t result;
    bool prime_n = (n < is_prime_cap) ? is_prime[(size_t)n] : false;
    if (prime_n && n > 2) {
        /* find the index of prime n */
        size_t t = 0;
        for (; t < prime_cnt && prime_list[t] != n; ++t)
            ;
        result = compute_recursive((idx_t)(t + 1));
    } else {
        seq_t spf = smallest_prime_factor(n);
        idx_t r = (idx_t)spf;
        idx_t s = n / r;
        seq_t a_r = compute_recursive(r);
        seq_t a_s = compute_recursive(s);
        if ((r % 2 == 0) && (s % 2 == 0))
            result = a_r + a_s - (seq_t)1;
        else
            result = a_r + a_s;
    }
    cache[n] = result;
    computed[n] = 1;
    return result;
}

/* Public API */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_cache(n);
    ensure_primes(n);
    return compute_recursive(n);
}
