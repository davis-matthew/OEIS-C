/*
 * A087875  a[n] = pi[n-pi[n-1]] + a[n - a[n-2]], where pi(x) = number of
 *          primes <= x.
 * https://oeis.org/A087875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi[n-pi[n-1]] + a[n-a[n-2]], a(1)=1, a(2)=1 pi(n) =
 *            number of primes ≤ n. The sequence is defined for n ≥ 1.
 *            Implementation uses cached prime counting and recursive
 *            memoisation
 * Range:     u8 n <= 630, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* -------------------------------------------------------------
   Prime counting cache: pi_cache[n] = pi(n) for 0 ≤ n ≤ cache_cap.
   ------------------------------------------------------------- */
static seq_t *pi_cache   = NULL;
static idx_t  pi_cache_cap = 0;

/* Expand the prime cache to answer queries up to 'limit'. */
static void ensure_pi_cache(idx_t limit)
{
    if (limit <= pi_cache_cap) return;

    /* Allocate new cache (limit+1 entries) and copy existing data. */
    idx_t new_cap    = limit + 1;
    seq_t *new_cache = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
    if (!new_cache) return;                      /* abort on OOM */
    if (pi_cache) {
        memcpy(new_cache, pi_cache, (size_t)(pi_cache_cap + 1) * sizeof(seq_t));
        free(pi_cache);
    }
    pi_cache = new_cache;

    /* Simple sieve of Eratosthenes to fill pi_cache. */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)new_cap, 1);
    if (!is_prime) return;
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t j = p * p; j <= limit; j += p) is_prime[j] = 0;
    }

    seq_t cnt = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (i >= 2 && is_prime[i]) ++cnt;
        pi_cache[i] = cnt;
    }
    free(is_prime);
    pi_cache_cap = limit;
}

static inline seq_t pi_cached(idx_t n)
{
    ensure_pi_cache(n);
    return pi_cache[n];
}

/* -------------------------------------------------------------
   Sequence cache: a_cache[n] = a(n) for 1 ≤ n ≤ a_cache_cap.
   ------------------------------------------------------------- */
static seq_t *a_cache = NULL;
static idx_t  a_cache_cap = 0;      /* maximum index computed (≥0) */

static void ensure_a_cache(idx_t n)
{
    if (n <= a_cache_cap) return;   /* already computed */

    /* Save old capacity and allocate a larger buffer. */
    idx_t old_cap   = a_cache_cap;           /* last computed index */
    idx_t new_cap   = n + 1;                 /* array size needed */
    seq_t *new_cache = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
    if (!new_cache) return;                 /* abort on OOM */

    if (a_cache) {
        memcpy(new_cache, a_cache, (size_t)(old_cap + 1) * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_cache;
    a_cache_cap = n;                        /* last computed index */

    /* Base values.  They are set regardless of old_cap. */
    if (n >= 1) a_cache[1] = 1;
    if (n >= 2) a_cache[2] = 1;

    /* Compute missing terms. */
    idx_t start = old_cap + 1;
    if (start < 3) start = 3;               /* first non‑trivial term is 3 */

    for (idx_t i = start; i <= n; ++i) {
        /* term1 = pi[i - pi[i-1]] */
        seq_t pi_im1 = pi_cached(i - 1);
        idx_t idx1   = i - (idx_t)pi_im1;
        seq_t term1  = pi_cached(idx1);

        /* term2 = a[i - a[i-2]] */
        seq_t a_im2 = a_cache[i - 2];
        idx_t idx2  = i - (idx_t)a_im2;
        seq_t term2 = a_cache[idx2];

        a_cache[i] = term1 + term2;
    }
}

/* -------------------------------------------------------------
   Public API: compute a(n).
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n < 1 */
    ensure_a_cache(n);
    return a_cache[n];
}
