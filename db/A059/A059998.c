/*
 * A059998  Number of different primes occurring when n is expressed as
 *          p1+q1+r1 = ... = pk+qk+rk where pk,qk,rk are primes with pk <= qk
 *          <= rk.
 * https://oeis.org/A059998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = number of distinct primes that occur in any
 *            representation of n as the sum of three primes p ≤ q ≤ r
 * Range:     u8 n <= 1622, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a sieve of Eratosthenes on demand and
   enumerates all prime triples (p,q,r) with p ≤ q ≤ r and p+q+r = n.
   A temporary bitmap records which primes appeared in any valid
   representation; the final count is returned.  Results are memoised. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Prime infrastructure ---------- */
static unsigned char *prime_flag = NULL;   /* 1 if prime, 0 otherwise          */
static size_t prime_limit  = 0;            /* maximum index covered by prime_flag */

static int *prime_list   = NULL;          /* array of prime values             */
static size_t primes_count = 0;           /* number of primes in prime_list    */
static size_t primes_capacity = 0;        /* current allocation size           */

/* Rebuild the prime list from prime_flag */
static void rebuild_prime_list(void)
{
    size_t i;
    primes_count = 0;
    for (i = 2; i <= prime_limit; ++i)
        if (prime_flag[i]) ++primes_count;

    if (primes_capacity < primes_count) {
        int *tmp = (int *)realloc(prime_list, primes_count * sizeof(int));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        primes_capacity = primes_count;
    }

    i = 0;
    for (size_t j = 2; j <= prime_limit; ++j)
        if (prime_flag[j]) prime_list[i++] = (int)j;
}

/* Extend the sieve to cover all numbers up to limit (>= prime_limit). */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    size_t old_limit = prime_limit;
    prime_limit = (size_t)limit;

    unsigned char *new_flag = (unsigned char *)realloc(prime_flag, prime_limit + 1);
    if (!new_flag) exit(EXIT_FAILURE);
    prime_flag = new_flag;

    /* initialise new entries as prime */
    for (size_t i = old_limit + 1; i <= prime_limit; ++i) prime_flag[i] = 1;

    /* 0 and 1 are not primes */
    if (prime_limit >= 0) prime_flag[0] = 0;
    if (prime_limit >= 1) prime_flag[1] = 0;

    size_t sq = (size_t)sqrt((double)prime_limit);
    for (size_t i = 2; i <= sq; ++i) {
        if (!prime_flag[i]) continue;
        size_t start = i * i;
        if (start <= old_limit) {
            start = ((old_limit + 1 + i - 1) / i) * i;
            if (start < i * i) start = i * i;
        }
        for (size_t j = start; j <= prime_limit; j += i)
            prime_flag[j] = 0;
    }

    rebuild_prime_list();
}

/* ---------- Memoisation for a(n) ---------- */
static seq_t *cache_val   = NULL;   /* cached result for each n */
static unsigned char *cache_comp = NULL; /* 1 if computed */
static size_t cache_capacity  = 0;  /* max index currently stored */

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset = 1 */

    /* Extend cache if necessary */
    if ((size_t)n > cache_capacity) {
        size_t new_cap = (size_t)n;
        seq_t *new_val = (seq_t *)realloc(cache_val, (new_cap + 1) * sizeof(seq_t));
        unsigned char *new_comp = (unsigned char *)realloc(cache_comp, (new_cap + 1) * sizeof(unsigned char));
        if (!new_val || !new_comp) exit(EXIT_FAILURE);
        for (size_t i = cache_capacity + 1; i <= new_cap; ++i) new_comp[i] = 0;
        cache_val   = new_val;
        cache_comp = new_comp;
        cache_capacity = new_cap;
    }

    if (cache_comp[n]) return cache_val[n];

    /* Ensure we have primes up to n. */
    ensure_primes((idx_t)n);

    /* Temporary array to mark primes that appear in any valid triple. */
    unsigned char *used = (unsigned char *)malloc((size_t)n + 1);
    if (!used) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)n; ++i) used[i] = 0;

    /* Iterate over primes p ≤ q ≤ r. */
    size_t p_cnt = primes_count;
    for (size_t pi = 0; pi < p_cnt; ++pi) {
        int p = prime_list[pi];
        if ((size_t)p > n) break;

        for (size_t qi = pi; qi < p_cnt; ++qi) {
            int q = prime_list[qi];
            if (p + 2u * q > n) break;          /* p + 2*q > n  ->  r < q */

            int r = (int)(n - p - q);
            if (r < q) continue;                /* safety for any rounding issues */
            if (r <= (int)prime_limit && prime_flag[r]) {
                used[p] = 1;
                used[q] = 1;
                used[r] = 1;
            }
        }
    }

    /* Count distinct primes that appeared. */
    unsigned int count = 0;
    for (int i = 2; i <= (int)n; ++i)
        if (used[i]) ++count;

    free(used);
    cache_val[n]   = (seq_t)count;
    cache_comp[n] = 1;
    return cache_val[n];
}
