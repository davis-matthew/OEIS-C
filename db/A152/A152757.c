/*
 * A152757  Numbers k such that the deficiency of k plus the number of proper
 *          divisors of k is a prime number (see A152864).
 * https://oeis.org/A152757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer k such that * 2*k - σ(k) +
 *            τ(k) - 1 is a prime number (deficiency + proper divisors)
 * Range:     u8 n <= 12, u16 n <= 927, u32 n <= 9466, u64 n <= 9466,
 *            u128 n <= 9466, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* --- Smallest prime factor table ------------------------------------ */
static uint32_t *spf_table = NULL;
static idx_t spf_limit = 0;
static bool spf_built = false;

/* Build SPF table up to a generous fixed limit (≥ 2 million). */
static void build_spf(void)
{
    if (spf_built) return;
    const idx_t INIT_LIMIT = 2000000;
    spf_table = (uint32_t *)calloc(INIT_LIMIT + 1, sizeof(uint32_t));
    if (!spf_table) abort();
    for (idx_t i = 2; i <= INIT_LIMIT; ++i) {
        if (spf_table[i] == 0) {
            spf_table[i] = (uint32_t)i;
            if ((idx_t)i * i <= (idx_t)INIT_LIMIT) {
                for (idx_t j = i * i; j <= (idx_t)INIT_LIMIT; j += i)
                    if (spf_table[j] == 0) spf_table[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = INIT_LIMIT;
    spf_built = true;
}

/* --- Compute σ(k) and τ(k) ------------------------------------------ */
static void sigma_tau(idx_t n, seq_t *out_sigma, size_t *out_tau)
{
    seq_t sigma = 1;
    size_t tau   = 1;
    idx_t temp   = n;
    while (temp > 1) {
        idx_t p = spf_table[temp];
        if (p == 0) p = temp; /* safety fallback, should not happen */
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        /* compute p^(exp+1) */
        seq_t p_pow = 1;
        for (int i = 0; i < exp + 1; ++i) p_pow *= (seq_t)p;
        seq_t sum_p = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= sum_p;
        tau   *= (size_t)(exp + 1);
    }
    *out_sigma = sigma;
    *out_tau   = tau;
}

/* --- Memoised sequence cache --------------------------------------- */
static seq_t *seq_cache = NULL;     /* stored values */
static size_t cache_len  = 0;       /* number of stored values */
static size_t cache_cap  = 0;       /* allocation capacity */
static idx_t last_k      = 1;       /* largest k processed so far */

/* Ensure cache has at least 'need' capacity. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    seq_cache = new_ptr;
    cache_cap = new_cap;
}

/* --- Main function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) abort();             /* offset starts at 1 */
    if (!spf_built) build_spf();

    /* Extend cache until we have at least n elements. */
    while (cache_len < n) {
        ++last_k;                    /* next candidate integer k */

        seq_t sigma;
        size_t tau;
        sigma_tau((idx_t)last_k, &sigma, &tau);

        /* Use signed arithmetic for deficiency and sum. */
        int64_t deficiency = (int64_t)2 * last_k - (int64_t)sigma;
        int64_t sum        = deficiency + (int64_t)(tau - 1);

        if (sum >= 2 && is_prime((idx_t)sum)) {
            ensure_cache_cap(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)last_k;
        }
    }

    return seq_cache[n - 1];
}
