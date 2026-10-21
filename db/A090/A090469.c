/*
 * A090469  Least (distinct) composite k such that k+n, k+2n, k+3n, ... up to
 *          n terms are all composite.
 * https://oeis.org/A090469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least unused composite k such that gcd(n,k) > 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"


static seq_t *cache = NULL;          /* stored values of a(1), a(2), … */
static idx_t  cache_size = 0;        /* number of terms in cache */

static bool *used = NULL;            /* used[k] = true when k has already appeared */
static idx_t used_cap = 0;           /* allocated size of used array */

/* For each prime p, stores the smallest multiple of p that has not yet
 * been proved used.  Indexed directly by the prime value. */
static idx_t *next_mult = NULL;
static idx_t next_mult_cap = 0;

static uint64_t *prime_list = NULL;  /* list of primes up to prime_limit */
static size_t   prime_cnt  = 0;
static idx_t    prime_limit = 0;

/* ------------------------------------------------------------------ */
static void ensure_used_capacity(idx_t M)
{
    if (used_cap >= M) return;
    bool *new_used = (bool *)calloc(M, sizeof(bool));
    if (!new_used) abort();
    if (used) {
        memcpy(new_used, used, used_cap * sizeof(bool));
        free(used);
    }
    used = new_used;
    used_cap = M;
}

/* ------------------------------------------------------------------ */
static void ensure_next_mult_capacity(idx_t M)
{
    if (next_mult_cap >= M) return;
    idx_t *new_next = (idx_t *)calloc(M, sizeof(idx_t));
    if (!new_next) abort();
    if (next_mult) {
        memcpy(new_next, next_mult, next_mult_cap * sizeof(idx_t));
        free(next_mult);
    }
    next_mult = new_next;
    next_mult_cap = M;
}

/* ------------------------------------------------------------------ */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_list) free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Retreive the smallest unused composite multiple of prime p. */
static idx_t next_unused_mul(idx_t p)
{
    ensure_next_mult_capacity(p + 1);
    if (next_mult[p] == 0) next_mult[p] = p * 2U;   /* start at 2p */

    while (true) {
        idx_t cand = next_mult[p];
        ensure_used_capacity(cand + 1);
        if (!used[cand]) break;
        next_mult[p] = cand + p;
    }
    return next_mult[p];
}

/* ------------------------------------------------------------------ */
/* Compute the n-th term of the sequence.  The cache and used sets are
 * extended only as far as needed. The computation is done in increasing
 * order so that previous terms are always available. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined index, return 0 */

    /* Expand the cache if required */
    if (cache_size < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        /* zero the new cache entries */
        for (idx_t i = cache_size; i < n; ++i) cache[i] = 0;

        /* Compute missing terms in natural order */
        for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
            seq_t val;
            if (idx == 1) {
                val = (seq_t)8;                 /* a(1) = 8 */
            } else {
                /* factorise idx into distinct primes */
                ensure_primes(idx);
                idx_t temp = idx;
                idx_t factors[64];
                size_t fac_cnt = 0;

                for (size_t i = 0; i < prime_cnt &&
                     (idx_t)prime_list[i] * (idx_t)prime_list[i] <= temp; ++i) {
                    if (temp % prime_list[i] == 0) {
                        factors[fac_cnt++] = (idx_t)prime_list[i];
                        while (temp % prime_list[i] == 0) temp /= prime_list[i];
                    }
                }
                if (temp > 1 && fac_cnt < 64) factors[fac_cnt++] = temp;

                /* choose smallest unused composite multiple of any factor */
                idx_t best = 0;
                for (size_t i = 0; i < fac_cnt; ++i) {
                    idx_t cand = next_unused_mul(factors[i]);
                    if (best == 0 || cand < best) best = cand;
                }
                val = (seq_t)best;
            }

            /* Mark k as used and store in cache */
            ensure_used_capacity((idx_t)val + 1);
            used[(idx_t)val] = true;
            cache[idx - 1] = val;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
