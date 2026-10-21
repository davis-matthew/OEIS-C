/*
 * A058354  Coefficients in the series (1 + x - x^4 - x^6 - x^8 - x^9 - x^10 -
 *          x^12 - x^14 ...)/(1 - x^2 - x^3 - x^5 - x^7 - x^11 - x^13 ...).
 * https://oeis.org/A058354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A058354 – coefficients of (1 + x – x⁴ – x⁶ – x⁸ – x⁹ – …) /
 *          (1 – x² – x³ – x⁵ – x⁷ – …)
 *  Recurrence:
 *      a(0) = 1,  a(1) = 1
 *      For n ≥ 2:
 *          a(n) = Σ_{p prime ≤ n} a(n – p)  – 1   if n is composite (n ≥ 4)
 *          a(n) = Σ_{p prime ≤ n} a(n – p)        otherwise
 *
 *  The denominator’s prime‑shift and the numerator’s composite‑shift
 *  together give the “–1” term on composites.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;          /* cached values a[0..size-1]        */
static idx_t  a_cache_size = 0;        /* current capacity                   */
static idx_t  a_cache_done  = 0;       /* next index not yet computed */

static uint8_t *is_prime = NULL;       /* 1 → prime, 0 → composite/0/1      */
static uint64_t *prime_list = NULL;    /* list of primes ≤ current limit    */
static size_t   prime_count = 0;       /* number of primes in prime_list    */
static idx_t    prime_limit = 0;       /* upper bound for primes built      */

/* Build a fresh prime sieve and list up to "limit" (inclusive). */
static void build_primes(idx_t limit)
{
    size_t N = (size_t)(limit + 1);

    /* allocate & initialize is_prime array */
    uint8_t *new_is = (uint8_t*)malloc(N * sizeof(uint8_t));
    if (!new_is) exit(1);
    for (size_t i = 0; i < N; ++i) new_is[i] = 1;
    if (N > 0) new_is[0] = 0;
    if (N > 1) new_is[1] = 0;

    /* simple sieve */
    for (size_t i = 2; i * i < N; ++i)
        if (new_is[i])
            for (size_t j = i * i; j < N; j += i)
                new_is[j] = 0;

    /* collect primes into list */
    size_t cnt = 0;
    for (size_t i = 2; i < N; ++i)
        if (new_is[i]) ++cnt;

    uint64_t *new_pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_pr) { free(new_is); exit(1); }

    size_t idx = 0;
    for (size_t i = 2; i < N; ++i)
        if (new_is[i])
            new_pr[idx++] = (uint64_t)i;

    /* swap old to new */
    free(is_prime);
    free(prime_list);
    is_prime   = new_is;
    prime_list = new_pr;
    prime_count = cnt;
    prime_limit = limit;
}

/* Ensure cache has room for indices up to "need". */
static void ensure_cache(idx_t need)
{
    if (a_cache_size > need) return;

    size_t new_size = (size_t)(need + 1);
    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache) exit(1);

    if (a_cache) {
        memcpy(new_cache, a_cache, a_cache_size * sizeof(seq_t));
        free(a_cache);
    }

    /* zero‑initialize the new region */
    memset(new_cache + a_cache_size,
           0,
           (new_size - a_cache_size) * sizeof(seq_t));

    a_cache = new_cache;
    a_cache_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Prepare data structures up to n */
    ensure_cache(n);
    if (prime_limit < n) build_primes(n);

    /* Compute missing a(i) values */
    for (idx_t i = a_cache_done; i <= n; ++i) {
        if (i == 0) { a_cache[i] = (seq_t)1; continue; }
        if (i == 1) { a_cache[i] = (seq_t)1; continue; }

        seq_t sum = (seq_t)0;
        /* sum over primes ≤ i */
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint64_t p = prime_list[pi];
            if (p > (uint64_t)i) break;
            sum += a_cache[i - (idx_t)p];
        }

        /* subtract 1 on composites (i ≥ 4) */
        if (i >= 4 && !is_prime[i])
            sum -= (seq_t)1;

        a_cache[i] = sum;
    }
    a_cache_done = n + 1;
    return a_cache[n];
}
