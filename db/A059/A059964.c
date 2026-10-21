/*
 * A059964  a(1) = 1, a(2) = 1, a(n) = a(n-p) + a((n+1)-p), n > 2, where p is
 *          the largest prime less than n.
 * https://oeis.org/A059964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(n) = a(n-p) + a((n+1)-p) (n>2), where p is
 *            the largest prime less than n. This implementation memoises
 *            computed values and generates primes dynamically as needed
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

/* ---------- Prime handling ---------- */
static idx_t   *prime_list = NULL; /* increasing list of primes */
static size_t   prime_cnt  = 0;    /* number of primes stored */
static size_t   prime_cap  = 0;    /* allocated capacity             */

/* Ensure that all primes ≤ limit are present in prime_list. */
static void ensure_primes(idx_t limit)
{
    if (limit < 2) return;                     /* no primes needed */

    /* initialise with the first prime if list is empty */
    if (prime_cnt == 0) {
        prime_cap = 16;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        if (!prime_list) return;
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    /* already have enough primes */
    if (limit <= prime_list[prime_cnt - 1]) return;

    idx_t start = prime_list[prime_cnt - 1] + 1;
    for (idx_t n = start; n <= limit; ++n) {
        idx_t root = int_sqrt(n);
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= root; ++i) {
            if (n % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;                     /* double capacity */
                idx_t *tmp = (idx_t *)realloc(prime_list,
                                              prime_cap * sizeof(idx_t));
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = n;
        }
    }
}

/* Return the largest prime less than n (n>2). */
static idx_t largest_prime_less_than(idx_t n)
{
    ensure_primes(n - 1);

    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return prime_list[lo - 1];
}

/* ---------- Sequence caching ---------- */
static seq_t *seq_cache = NULL;  /* 1‑based indices; index 0 unused */
static idx_t  cache_cap  = 0;
static idx_t  cache_end  = 0;

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)1;

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 3;                /* indices 0,1,2 valid */
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;
        seq_cache[0] = 0;             /* unused dummy */
        seq_cache[1] = 1;
        seq_cache[2] = 1;
        cache_end = 2;
    }

    /* grow cache if necessary */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        seq_cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute values up to n if not yet done */
    if (cache_end < n) {
        ensure_primes(n - 1);

        for (idx_t i = cache_end + 1; i <= n; ++i) {
            idx_t p  = largest_prime_less_than(i);
            idx_t i1 = i - p;          /* ≥ 1 */
            idx_t i2 = i + 1 - p;      /* ≥ 1 */
            seq_t v1 = seq_cache[i1];
            seq_t v2 = seq_cache[i2];
            seq_cache[i] = v1 + v2;
        }
        cache_end = n;
    }

    return seq_cache[n];
}
