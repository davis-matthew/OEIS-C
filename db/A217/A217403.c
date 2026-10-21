/*
 * A217403  a(n) is the smallest possible largest prime factor of the
 *          difference of two prime numbers q > p such that p + q = 2n.
 * https://oeis.org/A217403
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{ largest prime factor of (q - p) : * p < q are primes
 *            , p + q = 2n } (OEIS A217403)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches a sieve of primes up to the largest
 * 2n encountered, uses binary search to test membership of q, and
 * computes the largest prime factor of each difference by trial
 * division with the cached prime list.  It works for any seq_t that
 * can hold the returned value. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached prime list data */
static uint64_t *g_primes = NULL;   /* sorted list of primes */
static size_t    g_primes_cnt = 0;   /* number of primes */
static uint64_t  g_sieve_limit = 0;  /* maximum value sieved so far */

/* Simple sieve of Eratosthenes that stores all primes <= limit. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= g_sieve_limit) return;

    /* Allocate a bitset for primality */
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    /* Allocate and fill prime array */
    uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) exit(1);

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[idx++] = i;

    free(is_prime);
    free(g_primes);
    g_primes = new_primes;
    g_primes_cnt = cnt;
    g_sieve_limit = limit;
}

/* Binary search: returns true if x is in g_primes */
static bool is_prime_cached(uint64_t x)
{
    if (x > g_sieve_limit) return false;  /* out of range */
    size_t lo = 0, hi = g_primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (g_primes[mid] == x) return true;
        if (g_primes[mid] < x) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* Largest prime factor of n (n > 1).  n is uint64_t. */
static uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t lpf = 1;
    for (size_t i = 0; i < g_primes_cnt && g_primes[i] * g_primes[i] <= n; ++i) {
        uint64_t p = g_primes[i];
        if (n % p == 0) {
            lpf = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) lpf = n;  /* remaining prime factor */
    return lpf;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;      /* undefined for n < 4 */

    uint64_t N = (uint64_t)n;
    uint64_t twoN = N * 2ULL;

    /* Build prime table up to 2*n */
    ensure_primes(twoN);

    uint64_t best = 0; /* 0 indicates not set yet */

    /* Enumerate Goldbach partitions p + q = 2n, p < q, p prime */
    for (size_t i = 0; i < g_primes_cnt && g_primes[i] <= N; ++i) {
        uint64_t p = g_primes[i];
        uint64_t q = twoN - p;
        if (q <= p) continue;            /* ensure p < q */
        if (!is_prime_cached(q)) continue;/* q must be prime */

        uint64_t diff = q - p;
        uint64_t lpf  = largest_prime_factor(diff);

        if (best == 0 || lpf < best) best = lpf;
        if (best == 2) break;             /* cannot be smaller */
    }

    /* For n >= 4 a Goldbach partition always exists, so best > 0 */
    return (seq_t)best;
}
