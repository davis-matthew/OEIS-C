/*
 * A175192  a(n) = characteristic function of numbers k such that A000203(m) =
 *          k has solution, where A000203(m) = sums of divisors of m.
 * https://oeis.org/A175192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃m: σ(m) = n (sum of divisors of m), 0 otherwise.
 *            The routine builds a cache of all σ(m) for m ≤ N and marks all
 *            values that appear. For each call n ≥ 1 it guarantees that σ(m)
 *            is computed for all m up to n‑1, then returns whether n has been
 *            seen
 * Range:     u8 n <= 517, u16 n <= 9976, u32 n <= 9976, u64 n <= 9976,
 *            u128 n <= 9976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static caches */
static bool *cache_has_sigma = NULL;      /* cache_has_sigma[n] == true ⇔ ∃m:σ(m)=n */
static size_t cache_cap = 0;              /* allocated size of cache_has_sigma   */
static uint64_t *spf = NULL;              /* smallest prime factor array         */
static size_t spf_cap = 0;                /* size of spf array                   */

/* extend caches to accommodate n (inclusive) */
static void extend_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;           /* need indices 0 … n            */
    if (cache_cap < new_cap) {
        bool *new_cache = (bool*)malloc(new_cap * sizeof(bool));
        if (!new_cache) abort();
        if (cache_has_sigma) {
            memcpy(new_cache, cache_has_sigma, cache_cap * sizeof(bool));
            free(cache_has_sigma);
        }
        memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(bool));
        cache_has_sigma = new_cache;
        cache_cap = new_cap;
    }

    /* rebuild spf array up to new_cap */
    uint64_t *new_spf = (uint64_t*)calloc(new_cap, sizeof(uint64_t));
    if (!new_spf) abort();
    if (spf) {
        free(spf);
    }
    spf = new_spf;
    spf_cap = new_cap;
    for (size_t i = 2; i < new_cap; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            for (size_t j = i; j < new_cap; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* compute σ(m) for every m in [1 .. new_cap-1] and mark cache */
    for (uint64_t m = 1; m < new_cap; ++m) {
        seq_t sigma = 1;                     /* σ(1) = 1 */
        uint64_t mm = m;
        while (mm > 1) {
            uint64_t p = spf[mm];
            seq_t term = 1;                  /* (p^0)+(p^1)+...+(p^k) */
            seq_t power = 1;                  /* p^k   */
            while (mm % p == 0) {
                mm /= p;
                power *= (seq_t)p;
                term += power;
            }
            sigma *= term;
        }
        if ((seq_t)sigma < (seq_t)new_cap) {
            cache_has_sigma[(size_t)sigma] = true;
        }
    }
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, undefined for 0 */
    if ((size_t)n >= cache_cap) {
        extend_cache(n);
    }
    return cache_has_sigma[(size_t)n] ? (seq_t)1 : (seq_t)0;
}
