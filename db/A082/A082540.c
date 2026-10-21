/*
 * A082540  Number of ordered quadruples (a,b,c,d) with gcd(a,b,c,d)=1 (1 <=
 *          {a,b,c,d} <= n).
 * https://oeis.org/A082540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(k)*⌊n/k⌋^4 (OEIS A082540)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 261, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Internal helper: generate all primes up to a given limit using a
   simple sieve.  The resulting array is kept static and refreshed
   only when a larger limit is requested.
   ------------------------------------------------------------------ */
static uint64_t *primes_arr = NULL;
static size_t primes_cnt  = 0;
static idx_t   primes_lim = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= primes_lim)
        return;

    free(primes_arr);
    if (limit < 2) {
        primes_arr = NULL;
        primes_cnt = 0;
        primes_lim = 0;
        return;
    }

    /* Sieve of Eratosthenes */
    size_t size = (size_t)limit + 1;
    char *is_comp = (char *)malloc(size);
    if (!is_comp) return; /* out of memory: silently abort */

    for (size_t i = 0; i < size; ++i) is_comp[i] = 0;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                is_comp[q] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Allocate and fill primes array */
    uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!arr) { free(is_comp); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) arr[idx++] = i;

    free(is_comp);
    primes_arr = arr;
    primes_cnt = cnt;
    primes_lim = limit;
}

/* ------------------------------------------------------------------
   Internal helper: compute the Möbius function for numbers up to n.
   We use a static cache so that repeated calls are fast.
   ------------------------------------------------------------------ */
static int8_t *mu_cache = NULL;    /* values -1,0,1 */
static idx_t   mu_cache_cap = 0;

static void ensure_mu_cache(idx_t n)
{
    if (n < mu_cache_cap)
        return;

    int8_t *new_cache = (int8_t *)realloc(mu_cache, (size_t)(n + 1) * sizeof(int8_t));
    if (!new_cache) return; /* out of memory: silently abort */

    /* Initialize new entries to 0 (unknown) */
    for (idx_t i = mu_cache_cap; i <= n; ++i)
        new_cache[i] = 0;   /* 0 = not computed yet; real 0 is a Möbius value, but we recompute later */

    mu_cache = new_cache;
    mu_cache_cap = n + 1;
}

static int8_t mobius_value(idx_t k)
{
    if (k == 0) return 0; /* undefined but safe */

    if (mu_cache && k < mu_cache_cap && mu_cache[k] != 0) {
        /* Cached value is non-zero; need to distinguish real 0 Möbius */
        if (mu_cache[k] != 0 || (mu_cache[k] == 0 && k == 1)) /* k==1 Möbius=1 */
            return mu_cache[k];
    }

    idx_t temp = k;
    int8_t mu = 1;
    idx_t limit = (idx_t)(temp > 1 ? (idx_t)((double)temp * 0.5 + 0.5) : 0);

    /* Ensure primes up to sqrt(k) */
    ensure_primes(limit);

    for (size_t i = 0; i < primes_cnt && primes_arr[i] <= limit; ++i) {
        uint64_t p = primes_arr[i];
        if (temp % p == 0) {
            temp /= p;
            if (temp % p == 0) {
                mu = 0;
                break;
            }
            mu = -mu;
            int count = 2;
            while (temp % p == 0) {
                temp /= p;
                ++count;
                if (count > 2) { mu = 0; break; }
            }
            if (mu == 0) break;
            limit = (idx_t)(temp > 1 ? (idx_t)((double)temp * 0.5 + 0.5) : 0);
            if (mu == 0) break;
        }
    }

    if (mu != 0 && temp > 1) {
        /* Remaining prime factor > sqrt(original k) */
        mu = -mu;
    }

    /* Cache the result; store as -1, 0, or 1 */
    if (mu_cache && k < mu_cache_cap)
        mu_cache[k] = mu;
    return mu;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1 */

    /* Ensure caches large enough */
    ensure_mu_cache(n);

    /* Accumulate result in signed 64‑bit */
    long long acc = 0;

    for (idx_t k = 1; k <= n; ++k) {
        int8_t mu = mobius_value(k);
        if (mu == 0) continue;
        idx_t f = n / k;
        seq_t f4 = (seq_t)f * f * f * f;
        acc += (long long)mu * (long long)f4;
    }

    return (seq_t)acc;
}
