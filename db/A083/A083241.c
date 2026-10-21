/*
 * A083241  a(n) + a(n-1) + a(n-2) + a(n-3) = prime(n), n>2,
 *          a(0)=a(1)=a(2)=0;.
 * https://oeis.org/A083241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - a(n-1) - a(n-2) - a(n-3) with initial values
 *            a(0)=0, a(1)=0, a(2)=0
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Cache for the sequence values */
static seq_t *cache      = NULL;   /* memoised a(i) values          */
static size_t cache_cap  = 0;      /* current cache capacity        */
static idx_t  cache_computed = 2;  /* highest n that has been computed */

/* Cache for primes: prime_vals[0] = 2 = prime(1) */
static idx_t *prime_vals = NULL;   /* array of primes up to current size   */
static size_t prime_cap = 0;       /* capacity of prime_vals array         */
static size_t prime_cnt = 0;       /* number of primes stored              */
static idx_t   prime_candidate = 3;/* next odd number to test for primality */

/* Expand the prime array to at least min_cap entries */
static void expand_prime_array(size_t min_cap)
{
    if (prime_cap >= min_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    idx_t *tmp = (idx_t *)realloc(prime_vals, new_cap * sizeof(idx_t));
    if (!tmp) return; /* allocation failure – unlikely in expected usage */
    prime_vals = tmp;
    prime_cap = new_cap;
}

/* Ensure that prime_vals contains at least 'need' primes
   (prime(1)=2, prime(2)=3, ...).  */
static void ensure_primes_to(size_t need)
{
    if (prime_cnt >= need) return;

    /* Initially add the first prime */
    if (prime_cnt == 0) {
        expand_prime_array(16);
        prime_vals[prime_cnt++] = 2;           /* prime(1) */
        if (prime_cnt >= need) return;
    }

    /* Test odd candidates for primality */
    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt &&
             prime_vals[j] * prime_vals[j] <= prime_candidate; ++j) {
            if (prime_candidate % prime_vals[j] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            expand_prime_array(prime_cnt + 1);
            prime_vals[prime_cnt++] = prime_candidate;
        }
        prime_candidate += 2;   /* skip even numbers */
    }
}

/* Ensure that the cache can hold indices up to n */
static void ensure_cache_capacity(idx_t n)
{
    if (!cache || cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    /* Initialise cache if first use */
    if (!cache) {
        size_t init_cap = n + 1;
        if (init_cap < 3) init_cap = 3;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = cache[1] = cache[2] = 0;
        cache_cap = init_cap;
        cache_computed = 2;
    }

    /* Grow cache if needed */
    ensure_cache_capacity(n);

    /* Ensure we have enough primes */
    ensure_primes_to((size_t)n);

    /* Compute missing terms up to n */
    if (cache_computed < n) {
        for (idx_t i = cache_computed + 1; i <= n; ++i) {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
            seq_t prime = (seq_t)prime_vals[i-1];   /* prime(i) */
            cache[i] = prime - sum;
        }
        cache_computed = n;
    }

    return cache[(size_t)n];
}
