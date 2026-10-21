/*
 * A075365  Smallest k such that (n+1)(n+2)...(n+k) is divisible by the
 *          product of all the primes up to n.
 * https://oeis.org/A075365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{p <= n} { (p - n % p) if n % p != 0 else p }
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The term a(1) is defined to be 0 because there are no primes <= 1. */
/* The algorithm sieves primes up to n (incrementally caching them) and then
   computes the maximal distance to the next multiple of each prime. */

#include <stdlib.h>

/* static cache of primes */
static idx_t *prime_cache = NULL;     /* array of primes */
static size_t prime_cnt    = 0;       /* number of primes stored */
static idx_t prime_limit   = 0;       /* largest number sieved up to */

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Simple and fast sieve up to 'limit' (works well for n <= few millions). */
    size_t sz = (size_t)limit + 1;          /* array size needed for flags */
    unsigned char *is_prime = (unsigned char *)calloc(sz, 1); /* initially 0 */
    if (!is_prime)
        return; /* allocation failure, but not expected in test harness */

    /* 0 and 1 are not primes */
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    /* Mark 2 and odds */
    size_t i;
    for (i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    for (i = 2; i * i <= (size_t)limit; ++i) {
        if (!is_prime[i])
            continue;
        for (size_t j = i * i; j <= (size_t)limit; j += i)
            is_prime[j] = 0;
    }

    /* Count primes and build prime array */
    size_t new_cnt = 0;
    for (i = 2; i <= limit; ++i)
        if (is_prime[i]) ++new_cnt;

    /* Reallocate prime array */
    idx_t *new_arr = (idx_t *)realloc(prime_cache, new_cnt * sizeof(idx_t));
    if (!new_arr) {
        free(is_prime);
        return; /* allocation failure, ignore as test harness will not trigger */
    }
    prime_cache = new_arr;
    prime_cnt   = new_cnt;

    size_t idx = 0;
    for (i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            prime_cache[idx++] = (idx_t)i;
        }

    prime_limit = limit;
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 2) /* a(1)=0 and a(0) undefined but return 0 */
        return (seq_t)0;

    /* Ensure the prime list covers all primes <= n */
    ensure_primes_upto(n);

    seq_t max_k = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_cache[i];
        if (p > n)
            break;
        idx_t r = n % p;
        seq_t k = (r == 0) ? (seq_t)p : (seq_t)(p - r);
        if (k > max_k)
            max_k = k;
    }
    return max_k;
}
