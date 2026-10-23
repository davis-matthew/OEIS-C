/*
 * A106933  Primes of the form x^2-xy+17y^2, with x and y nonnegative.
 * https://oeis.org/A106933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form x^2 - x*y + 17*y^2 (x,y ≥ 0)
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   cmp_seq, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/cmp_seq.h"

/* -------------------------------------------------------------- */
/* Comparer for qsort (works for any seq_t type) */

/* -------------------------------------------------------------- */
/* Global cache for primes of the required form */
static seq_t *prime_list = NULL;   /* sorted, unique primes */
static size_t prime_cnt  = 0;      /* number of primes stored  */
static idx_t  limit_used  = 0;     /* the maximal value checked so far */

/* -------------------------------------------------------------- */
/* Compute all such primes up to 'limit', store into the global cache */
static void recompute_primes(idx_t limit)
{
    /* Rough bounds for the loops */
    const double dlimit = (double)limit;
    const uint64_t y_max = (uint64_t)(2.0 * sqrt(dlimit / 67.0) + 2.0);
    const uint64_t x_max = (uint64_t)(sqrt(dlimit) + 2.0);

    /* Temporary buffer for found primes */
    size_t cap = 256;
    seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!tmp) abort();

    size_t tmp_cnt = 0;
    for (uint64_t y = 1; y <= y_max; ++y) {
        for (uint64_t x = 0; x <= x_max; ++x) {
            seq_t val = (seq_t)x * (seq_t)x
                      - (seq_t)x * (seq_t)y
                      + (seq_t)17 * (seq_t)y * (seq_t)y;
            if (val > limit) break;
            if (val < 2) continue;
            if (is_prime_uint64((uint64_t)val)) {
                if (tmp_cnt == cap) {
                    cap <<= 1;
                    tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                    if (!tmp) abort();
                }
                tmp[tmp_cnt++] = val;
            }
        }
    }

    /* Sort and deduplicate the temporary buffer */
    if (tmp_cnt > 1)
        qsort(tmp, tmp_cnt, sizeof(seq_t), cmp_seq);

    size_t uniq = 0;
    for (size_t i = 0; i < tmp_cnt; ++i) {
        if (uniq == 0 || tmp[i] != tmp[uniq-1]) {
            tmp[uniq++] = tmp[i];
        }
    }

    /* Store results into the global cache (replacing old data) */
    free(prime_list);
    prime_list = (seq_t *)malloc(uniq * sizeof(seq_t));
    if (!prime_list) abort();
    for (size_t i = 0; i < uniq; ++i)
        prime_list[i] = tmp[i];
    prime_cnt  = uniq;
    limit_used = limit;

    free(tmp);
}

/* -------------------------------------------------------------- */
/* Main function: return the n‑th term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of range – behaviour undefined */

    /* Ensure the cache contains at least n primes */
    while (prime_cnt < (size_t)n) {
        idx_t new_limit = limit_used ? (limit_used * 2 + 1000) : 1000;
        recompute_primes(new_limit);
    }

    return prime_list[n-1];
}
