/*
 * A106933  Primes of the form x^2-xy+17y^2, with x and y nonnegative.
 * https://oeis.org/A106933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form x^2 - x*y + 17*y^2 (x,y ≥ 0)
 * Range:     u8 none, u16 n <= 2126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9018 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

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
    seq_t *tmp = mem_new(seq_t, cap);
    if (!tmp) abort();

    size_t tmp_cnt = 0;
    for (uint64_t y = 1; y <= y_max; ++y) {
        for (uint64_t x = 0; x <= x_max; ++x) {
            seq_t val = (seq_t)x * (seq_t)x
                      - (seq_t)x * (seq_t)y
                      + (seq_t)17 * (seq_t)y * (seq_t)y;
            if (val > limit) break;
            if (val < 2) continue;
            if (is_prime_u64((uint64_t)val)) {
                if (tmp_cnt == cap) {
                    cap <<= 1;
                    tmp = mem_resize(seq_t, tmp, cap);
                    if (!tmp) abort();
                }
                tmp[tmp_cnt++] = val;
            }
        }
    }

    /* Sort and deduplicate the temporary buffer */
    if (tmp_cnt > 1)
        mem_qsort(tmp, tmp_cnt, sizeof(seq_t), cmp_seq);

    size_t uniq = 0;
    for (size_t i = 0; i < tmp_cnt; ++i) {
        if (uniq == 0 || tmp[i] != tmp[uniq-1]) {
            tmp[uniq++] = tmp[i];
        }
    }

    /* Store results into the global cache (replacing old data) */
    mem_free(prime_list);
    prime_list = mem_new(seq_t, uniq);
    if (!prime_list) abort();
    for (size_t i = 0; i < uniq; ++i)
        prime_list[i] = tmp[i];
    prime_cnt  = uniq;
    limit_used = limit;

    mem_free(tmp);
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
