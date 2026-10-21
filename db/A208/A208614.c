/*
 * A208614  Number of partitions of n into distinct primes where all of the
 *          prime factors of n are represented in the partition.
 * https://oeis.org/A208614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 360, u32 n <= 1182, u64 n <= 4095,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into distinct primes that include all
   prime divisors of n.  For each n ≥ 0, return the count. 
   The implementation performs prime factorisation, then a subset‑sum DP
   over the remaining primes.  It lazily builds a sieve of primes up to
   the largest n requested.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -----------  global cache for primes  ----------- */
static uint64_t *prime_list   = NULL; /* array of primes */
static size_t   prime_count   = 0;    /* number of primes stored */
static size_t   prime_limit   = 0;    /* greatest n for which primes are known */

/* ensure that prime_list contains all primes ≤ limit, reallocating if necessary */
static void ensure_primes_upto (idx_t limit)
{
    if (limit <= prime_limit) return;

    size_t n = (size_t)limit;
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_list = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!new_list) exit(1);
    prime_list = new_list;
    prime_count = cnt;

    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint64_t)i;

    prime_limit = n;
    free(is_prime);
}

/* ========= smallest prime factor (trial division) ========= */
static uint64_t spf (uint64_t n)
{
    if (n <= 3) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return i;
    return n;        /* n is prime */
}

/* ------------------  main function -------------------- */
seq_t a (idx_t n)
{
    /* offset 0 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* build primes up to n on demand */
    ensure_primes_upto(n);

    /* factor n to obtain distinct prime factors */
    uint64_t nn   = (uint64_t)n;
    uint64_t req[8];   /* n ≤ 200000 → at most 8 distinct primes */
    int req_cnt = 0;
    uint64_t last_p = 0;
    uint64_t sum_req = 0;

    while (nn > 1) {
        uint64_t p = spf(nn);
        if (p != last_p) {               /* new distinct prime */
            req[req_cnt++] = p;
            sum_req += p;
            last_p = p;
        }
        while (nn % p == 0) nn /= p;
    }
    if (sum_req > n) return (seq_t)0;    /* impossible */

    int residual = (int)(n - sum_req);
    if (residual == 0) return (seq_t)1;  /* only required primes form the partition */

    /* subset‑sum DP */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;            /* allocation failure fallback */

    for (int i = 0; i <= (int)n; ++i) dp[i] = 0;
    dp[0] = 1;                            /* empty set */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > (uint64_t)n) break;

        /* skip primes that are already required */
        bool skip = false;
        for (int k = 0; k < req_cnt; ++k)
            if (p == req[k]) { skip = true; break; }
        if (skip) continue;

        for (int s = (int)n; s >= (int)p; --s)
            dp[s] = dp[s] + dp[s - (int)p];
    }

    seq_t result = dp[residual];
    free(dp);
    return result;
}
