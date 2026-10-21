/*
 * A065861  Remainder when the n-th composite number is divided by pi(n), the
 *          number of primes not exceeding n.
 * https://oeis.org/A065861
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A065861:  a(n) = A002808(n) mod A000720(n)
 *  where  A002808(n) = n-th composite number
 *         A000720(n) = number of primes ≤ n
 *  Offset: 2  →  a(2) is the first term shown in OEIS.
 */

/*  Included headers  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/*  Static state – lists of primes and composites up to the current limit  */
static idx_t sieve_limit = 0;

/*  C version – dynamic arrays  */
#ifndef __cplusplus
static seq_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static seq_t *composites = NULL;
static size_t composites_cnt = 0;
static size_t composites_cap = 0;
#endif

/*  C++ version – std::vector  */
#ifdef __cplusplus
static std::vector<seq_t> primes_vec;
static std::vector<seq_t> composites_vec;
#endif

/*  Ensure capacity for the C array of primes  */
#ifndef __cplusplus
static void ensure_prime_cap(size_t need)
{
    if (primes_cap < need) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
}
static void ensure_composite_cap(size_t need)
{
    if (composites_cap < need) {
        size_t new_cap = composites_cap ? composites_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        composites = tmp;
        composites_cap = new_cap;
    }
}
#endif

/*  Build sieve up to new_limit – rebuilds prime and composite lists  */
static void build_limit(idx_t new_limit)
{
    /* Allocate sieve array */
    unsigned char *is_prime = (unsigned char *)malloc((new_limit + 1) * sizeof(unsigned char));
    if (!is_prime) abort();
    memset(is_prime, 1, new_limit + 1);
    if (new_limit >= 0) { is_prime[0] = 0; }
    if (new_limit >= 1) { is_prime[1] = 0; }

    /* Classic Eratosthenes */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Generate primes */
#ifdef __cplusplus
    primes_vec.clear();
    primes_vec.reserve((size_t)(new_limit / 10 + 100));
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) primes_vec.push_back((seq_t)i);
    }
#else
    free(primes);
    primes = NULL;
    primes_cnt = 0;
    primes_cap = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) {
            ensure_prime_cap(primes_cnt + 1);
            primes[primes_cnt++] = (seq_t)i;
        }
    }
#endif

    /* Generate composites */
#ifdef __cplusplus
    composites_vec.clear();
    composites_vec.reserve((size_t)(new_limit / 2 + 100));
    for (idx_t i = 4; i <= new_limit; ++i) {
        if (!is_prime[i]) composites_vec.push_back((seq_t)i);
    }
#else
    free(composites);
    composites = NULL;
    composites_cnt = 0;
    composites_cap = 0;
    for (idx_t i = 4; i <= new_limit; ++i) {
        if (!is_prime[i]) {
            ensure_composite_cap(composites_cnt + 1);
            composites[composites_cnt++] = (seq_t)i;
        }
    }
#endif

    free(is_prime);
    sieve_limit = new_limit;
}

/*  Make sure we have at least n composites stored  */
static void ensure_composites(idx_t n)
{
    while (1) {
#ifdef __cplusplus
        if (composites_vec.size() >= n) return;
#else
        if (composites_cnt >= n) return;
#endif
        /* New limit: at least double the current, or large enough for n composites */
        idx_t new_limit = sieve_limit ? sieve_limit * 2 + 10 : 20;
        if (new_limit < n * 4 + 10) new_limit = n * 4 + 10;
        build_limit(new_limit);
    }
}

/*  Compute π(n): number of primes ≤ n  */
static seq_t pi_of(idx_t n)
{
#ifdef __cplusplus
    return (seq_t)(std::upper_bound(primes_vec.begin(), primes_vec.end(), (seq_t)n) - primes_vec.begin());
#else
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= (seq_t)n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
#endif
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* defined for n ≥ 2 only */

    /* Ensure we have at least n composites  */
    ensure_composites(n);

    /* Retrieve n-th composite number (1‑based)  */
    seq_t comp =
#ifdef __cplusplus
        composites_vec[n - 1];
#else
        composites[n - 1];
#endif

    /* Compute π(n)  */
    seq_t prime_cnt = pi_of(n);

    /* Return remainder  */
    return comp % prime_cnt;
}
