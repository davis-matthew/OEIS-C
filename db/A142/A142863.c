/*
 * A142863  Semiprimes n (A001358) for which A000001(n) is 2.
 * https://oeis.org/A142863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates semiprimes p^2 or p*q (p < q) with q ≡ 1 (mod p) and
   returns the nth term (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* cached sequence data */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static bool cache_built = false;

/* compare function for qsort (seq_t is an unsigned integer type) */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* build the cache of all terms up to a given numeric limit */
static void build_cache(idx_t limit)
{
    /* free old cache if any */
    if (cache) {
        free(cache);
        cache = NULL;
        cache_size = 0;
    }

    /* obtain list of primes up to limit */
    size_t primes_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(limit, &primes, &primes_cnt);
    if (!primes) return;      /* allocation failure – abort */

    /* storage for candidate numbers (dynamic array) */
    size_t cand_cap = 1024;
    seq_t *cand = (seq_t *)malloc(cand_cap * sizeof(seq_t));
    if (!cand) { free(primes); return; }
    size_t cand_len = 0;

    /* add prime squares p^2 <= limit */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t sq = p * p;
        if (sq > (uint64_t)limit) break;
        if (cand_len == cand_cap) {
            cand_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cand, cand_cap * sizeof(seq_t));
            if (!tmp) { free(cand); free(primes); return; }
            cand = tmp;
        }
        cand[cand_len++] = (seq_t)sq;
    }

    /* add products p*q where p < q, q ≡ 1 (mod p), and p*q <= limit */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        /* minimum possible q is next prime after p; if it's already > limit/p, skip */
        if (p * 2 > (uint64_t)limit) continue;
        for (size_t j = i + 1; j < primes_cnt; ++j) {
            uint64_t q = primes[j];
            uint64_t prod = p * q;
            if (prod > (uint64_t)limit) break;
            if (q % p == 1) {
                if (cand_len == cand_cap) {
                    cand_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cand, cand_cap * sizeof(seq_t));
                    if (!tmp) { free(cand); free(primes); return; }
                    cand = tmp;
                }
                cand[cand_len++] = (seq_t)prod;
            }
        }
    }

    free(primes);

    /* sort and deduplicate the candidate list */
    qsort(cand, cand_len, sizeof(seq_t), seq_cmp);
    size_t uniq_len = 0;
    for (size_t i = 0; i < cand_len; ++i) {
        if (i == 0 || cand[i] != cand[i - 1]) {
            cand[uniq_len++] = cand[i];
        }
    }

    /* shrink to actual size */
    seq_t *tmp = (seq_t *)realloc(cand, uniq_len * sizeof(seq_t));
    if (!tmp) {
        free(cand);
        return;
    }
    cand = tmp;

    cache = cand;
    cache_size = uniq_len;
    cache_built = true;
}

/* public function */
seq_t a(idx_t n)
{
    /* lazily build or extend cache until it contains n terms */
    if (!cache_built) {
        build_cache(200000);   /* initial limit */
    }
    while (n > cache_size) {
        /* increase search limit and rebuild */
        idx_t new_limit = cache_size * 10 + 2000;   /* heuristic growth */
        build_cache(new_limit);
        /* safety: if still not enough, double the limit */
        if (n > cache_size) {
            new_limit *= 2;
            build_cache(new_limit);
        }
    }
    if (n == 0 || n > cache_size) return (seq_t)0;
    return cache[n - 1];
}
