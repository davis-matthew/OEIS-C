/*
 * A134187  a(0)=1. a(n) = the number of terms of the sequence (from among
 *          terms a(0) through a(n-1)) which equal any "non-isolated divisors"
 *          of (2n). A divisor, k, of n is non-isolated if (k-1) or (k+1) also
 *          divides n.
 * https://oeis.org/A134187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of previous terms equal to any non‑isolated
 *            divisor of 2*n
 * Range:     u8 n <= 419, u16 n <= 9040, u32 n <= 9040, u64 n <= 9040,
 *            u128 n <= 9040, big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (non‑isolated: k−1 or k+1 also divides the number) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes.h"

/* cache for computed a(k) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t next_to_compute = 0;

/* frequency table of values that have appeared so far */
static seq_t *freq = NULL;
static size_t freq_cap = 0;

/* list of primes for factorization */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* ensure cache has room for indices 0 .. n */
static void ensure_cache_cap(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ensure freq has room for indices 0 .. n */
static void ensure_freq_cap(idx_t n)
{
    if (n < freq_cap) return;
    size_t new_cap = freq_cap ? freq_cap << 1 : 8;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    freq = (seq_t *)realloc(freq, new_cap * sizeof(seq_t));
    memset(freq + freq_cap, 0, (new_cap - freq_cap) * sizeof(seq_t));
    freq_cap = new_cap;
}

/* ensure the prime table covers 0 .. limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;
    if (primes) free(primes);
    build_primes(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

seq_t a(idx_t n)
{
    ensure_cache_cap(n);
    ensure_freq_cap(n);

    /* prime table needed only for n > 0 */
    if (n > 0) {
        idx_t need = (idx_t)( (uint64_t)n * 2 );
        if (primes_limit < need) ensure_primes(need);
    }

    /* compute missing terms up to n */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            freq[(idx_t)1] += (seq_t)1;
            continue;
        }

        uint64_t t = (uint64_t)i * 2ULL;

        /* factor t */
        uint64_t pf[32];
        uint64_t exp[32];
        size_t pcnt = 0;
        factor_u64(t, pf, exp, &pcnt);

        /* generate all divisors of t */
        uint64_t divs[256];
        size_t dsz = 1;
        divs[0] = 1ULL;
        for (size_t p = 0; p < pcnt; ++p) {
            uint64_t pval = pf[p];
            uint64_t mult = 1ULL;
            size_t cur = dsz;
            for (uint64_t e = 1; e <= exp[p]; ++e) {
                mult *= pval;
                for (size_t j = 0; j < cur; ++j) {
                    divs[dsz++] = divs[j] * mult;
                }
            }
        }

        /* sum frequencies of non‑isolated divisors */
        seq_t sum = 0;
        for (size_t j = 0; j < dsz; ++j) {
            uint64_t d = divs[j];
            bool nonisolated = false;
            if (d == 1ULL) {
                /* only d+1 can divide t */
                nonisolated = (t % 2ULL == 0);
            } else {
                if (t % (d - 1ULL) == 0) nonisolated = true;
                else if (t % (d + 1ULL) == 0) nonisolated = true;
            }
            if (nonisolated) {
                idx_t idx = (idx_t)d;
                if (idx < freq_cap) sum += freq[idx];
            }
        }

        cache[i] = sum;
        /* record this value for future frequency counting */
        idx_t idx_sum = (idx_t)sum;
        if (idx_sum < freq_cap) freq[idx_sum] += (seq_t)1;
    }

    next_to_compute = n + 1;
    return cache[n];
}
