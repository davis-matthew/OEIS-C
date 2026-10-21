/*
 * A070548  a(n) = Cardinality{ k in range 1 <= k <= n such that Moebius(k) =
 *          1 }.
 * https://oeis.org/A070548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Count_{k ≤ n} (μ(k) = 1)
 * Range:     u8 n <= 841, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here μ(n) is the Möbius function.          */
#include <stdlib.h>

static size_t  cache_n  = 0;      /* largest n processed so far   */
static uint64_t *spf   = NULL;    /* smallest prime factor array   */
static int      *mu    = NULL;    /* Möbius values memoised      */
static seq_t    *pref  = NULL;    /* prefix sums of μ=1          */

/* Grow the cached arrays to accommodate at least max_n entries. */
static void grow_cache(size_t max_n)
{
    /* Allocate a new larger array, recompute everything from scratch. */
    size_t i, n = max_n + 1;  /* include index 0 */
    uint64_t *new_spf = (uint64_t *)calloc(n, sizeof(uint64_t));
    int      *new_mu  = (int      *)malloc(n * sizeof(int));
    seq_t    *new_pref= (seq_t    *)malloc(n * sizeof(seq_t));

    if (!new_spf || !new_mu || !new_pref) return; /* allocation fail */

    new_spf[1] = 1;          /* not really used for 1 */
    for (i = 2; i < n; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;                     /* i is prime */
            if (i <= n / i)                     /* avoid overflow in i*i */
                for (uint64_t j = i * i; j < n; j += i)
                    if (!new_spf[j]) new_spf[j] = i;
        }
    }
    /* Möbius values: μ(1) = 1, μ(p) = -1 for prime p, μ(pq)=1 for distinct primes, etc. */
    new_mu[1] = 1;
    for (i = 2; i < n; ++i) {
        uint64_t p = new_spf[i];
        uint64_t m = i / p;
        new_mu[i] = (m % p == 0) ? 0 : -new_mu[m];
    }
    new_pref[0] = 0;
    for (i = 1; i < n; ++i) {
        seq_t val = new_pref[i-1];
        if (new_mu[i] == 1) val += (seq_t)1;
        new_pref[i] = val;
    }

    /* Replace old arrays. */
    free(spf);
    free(mu);
    free(pref);
    spf   = new_spf;
    mu    = new_mu;
    pref  = new_pref;
    cache_n = max_n;
}

/* Count of integers ≤ n with μ(k) = 1. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;  /* safety for out-of‑range calls */

    /* Extend cache if necessary. */
    if ((size_t)n > cache_n) {
        grow_cache((size_t)n);
        /* If allocation failed, return 0 to avoid crash. */
        if (pref == NULL) return 0;
    }
    return pref[(size_t)n];
}
