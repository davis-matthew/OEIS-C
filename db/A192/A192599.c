/*
 * A192599  Let N = pq (p<q) = A006881(n). Then a(n) = | (p^2-q)(q-1)/2 |.
 * https://oeis.org/A192599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |(p^2 - q)(q - 1)/2| for p*q = A006881(n), p < q
 * Range:     u8 n <= 10, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

typedef struct {
    uint64_t prod;
    uint32_t p;
    uint32_t q;
} _Semi;

static seq_t *cache   = NULL;   /* 1‑based cache of a(n) values   */
static idx_t  cache_len = 0;    /* number of stored terms          */
static idx_t  sieve_lim  = 0;   /* last prime‑sieve limit used     */

/* Compare semiprimes by product */
static int _cmp_prod(const void *A, const void *B)
{
    const _Semi *a = (const _Semi *)A;
    const _Semi *b = (const _Semi *)B;
    if (a->prod < b->prod) return -1;
    if (a->prod > b->prod) return  1;
    return 0;
}

/* Ensure the cache contains at least n terms */
static void _ensure_cache(idx_t n)
{
    if (n == 0 || n <= cache_len) return;

    idx_t limit = sieve_lim ? sieve_lim * 2 : 200; /* initial guess           */
    while (true) {
        /* Build primes up to `limit` */
        size_t   primes_cnt = 0;
        uint64_t *primes    = NULL;
        build_primes_upto(limit, &primes, &primes_cnt);
        if (primes == NULL || primes_cnt < 2) { /* safety */
            free(primes);
            limit *= 2;
            continue;
        }

        /* Generate all semiprimes with product ≤ limit */
        size_t sem_cap = 0, sem_cnt = 0;
        _Semi *sems = NULL;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            for (size_t j = i + 1; j < primes_cnt; ++j) {
                uint64_t q = primes[j];
                uint64_t prod = p * q;
                if (prod > limit) break;
                if (sem_cnt == sem_cap) {
                    sem_cap = sem_cap ? sem_cap * 2 : 16;
                    sems = (_Semi *)realloc(sems, sem_cap * sizeof(_Semi));
                    if (!sems) abort();
                }
                sems[sem_cnt].prod = prod;
                sems[sem_cnt].p    = (uint32_t)p;
                sems[sem_cnt].q    = (uint32_t)q;
                ++sem_cnt;
            }
        }
        free(primes);

        /* Sort semiprimes by product to obtain A006881 order */
        qsort(sems, sem_cnt, sizeof(_Semi), _cmp_prod);

        /* If we now have enough terms, build the cache */
        if (sem_cnt >= n) {
            seq_t *new_cache = (seq_t *)calloc((size_t)sem_cnt + 1, sizeof(seq_t));
            if (!new_cache) abort();
            new_cache[0] = 0; /* unused for 1‑based indexing */

            for (size_t k = 0; k < sem_cnt; ++k) {
                uint32_t p = sems[k].p;
                uint32_t q = sems[k].q;
                seq_t p2  = (seq_t)p * (seq_t)p;           /* p²          */
                seq_t diff = p2 >= (seq_t)q ? p2 - (seq_t)q
                                            : (seq_t)q - p2;/* |p² − q|   */
                seq_t mult = diff * ((seq_t)q - (seq_t)1);  /* (p² − q)(q−1) */
                new_cache[k + 1] = mult / (seq_t)2;        /* ½*…        */
            }

            /* Replace old cache */
            free(cache);
            cache       = new_cache;
            cache_len   = sem_cnt;
            sieve_lim   = limit;

            free(sems);
            break;
        }

        /* Not enough terms yet – discard work and double the limit */
        free(sems);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* OEIS offset starts at 1 */
    _ensure_cache(n);
    return cache[n];
}
