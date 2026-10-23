/*
 * A033476  Squares of primes or products of pairs of consecutive primes.
 * https://oeis.org/A033476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Squares of primes or products of consecutive primes (OEIS A033476) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Comparison function for qsort */
static int seq_compare(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Cached sequence and its length */
static seq_t *seq_cache = NULL;
static size_t  seq_len  = 0;

/* Ensure the cache contains at least (n_needed + 1) terms */
static void recompute_up_to(idx_t n_needed)
{
    /* Release previous cache */
    if (seq_cache) {
        free(seq_cache);
        seq_cache = NULL;
        seq_len   = 0;
    }

    idx_t limit = 100;                     /* initial prime bound */
    while (1) {
        idx_t *primes = NULL;
        size_t prime_cnt = 0;

        /* Build primes up to 'limit' */
        build_primes(limit, &primes, &prime_cnt);

        /* Estimate number of candidates: one square per prime plus
           one product for each consecutive pair (at most 2*prime_cnt) */
        size_t cand_cnt = prime_cnt * 2;
        if (prime_cnt == 0) {                    /* never happens for limit >= 2 */
            free(primes);
            limit *= 2;
            continue;
        }
        /* Allocate array for candidate numbers */
        seq_t *cand = (seq_t *)malloc(cand_cnt * sizeof(seq_t));
        if (!cand) { free(primes); abort(); }

        size_t pos = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            cand[pos++] = p * p;                 /* square of prime */
            if (i + 1 < prime_cnt) {
                seq_t q = (seq_t)primes[i + 1];
                cand[pos++] = p * q;             /* product of consecutive primes */
            }
        }
        /* In rare case difference between cnt*2 and actual used pos */
        cand_cnt = pos;

        /* If we have enough terms, sort and cache them */
        if (cand_cnt > (size_t)n_needed) {
            qsort(cand, cand_cnt, sizeof(seq_t), seq_compare);
            seq_cache = cand;
            seq_len   = cand_cnt;
            free(primes);
            return;
        }

        /* Not enough terms: clean up and increase bound */
        free(cand);
        free(primes);
        limit *= 2;
    }
}

/* Return the n-th term of the sequence (0‑based indexing) */
seq_t a(idx_t n)
{
    if (!seq_cache || seq_len <= (size_t)n) {
        recompute_up_to(n);
    }
    return seq_cache[(size_t)n];
}
