/*
 * A236416  Sequence of distinct least positive triangular numbers such that
 *          the arithmetic mean of the first n terms is also a triangular
 *          number. Initial term is 1.
 * https://oeis.org/A236416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: distinct least positive triangular numbers such that the arithmetic mean of the first n
   terms is also a triangular number.  The sequence starts with 1, 55, 28, 136, ... (OEIS A236416).
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ----  Static memoisation --------------------------------------------- */

static seq_t *cache      = NULL;      /* holds a(1) .. a(cache_len) */
static size_t cache_cap  = 0;         /* capacity of cache array   */
static size_t cache_len  = 0;         /* number of terms currently stored */

static bool *used_tri    = NULL;      /* bitmap of used triangular indices */
static size_t used_cap   = 0;         /* capacity of used_tri array */

/* Sum of all terms already computed */
static seq_t sum_last     = 0;

/* ----  helpers ------------------------------------------------------- */

static void ensure_cache_size(idx_t N)
{
    if ((size_t)N <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < (size_t)N)
        new_cap <<= 1;               /* double the capacity */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static void ensure_used_size(idx_t K)
{
    if ((size_t)K <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 8;
    while (new_cap < (size_t)K)
        new_cap <<= 1;               /* double the capacity */
    bool *tmp = (bool*)realloc(used_tri, new_cap * sizeof(bool));
    /* initialise new slots to false */
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used_tri = tmp;
    used_cap = new_cap;
}

/* Is x a triangular number? 8*x+1 must be a perfect square and odd. */
static bool is_triangular(seq_t x)
{
    seq_t d = 8 * x + 1;
    seq_t s = floor_sqrt(d);
    if (s * s != d)
        return false;
    return (s & (seq_t)1u) == (seq_t)1u;
}

/* ----  main function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, but avoid crash */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Compute all terms up to n (inclusive). */
    for (idx_t cur = (idx_t)(cache_len + 1); cur <= n; ++cur) {
        /* ----  base case a(1) ------------------------------------- */
        if (cur == 1) {
            ensure_cache_size(1);
            cache[0] = (seq_t)1;       /* T1 */
            cache_len = 1;
            sum_last = (seq_t)1;
            ensure_used_size(1);
            used_tri[1] = true;
            continue;
        }

        /* ----  find the smallest unused triangular number that
                keeps the average triangular ---------------------- */
        seq_t best_t = 0;
        idx_t  best_k = 0;

        for (idx_t k = 1; ; ++k) {
            ensure_used_size(k);
            if (used_tri[k])
                continue;               /* already in the sequence */

            seq_t t = ((seq_t)k * ((seq_t)k + (seq_t)1)) / (seq_t)2;   /* T_k */

            seq_t candidate_sum = sum_last + t;
            if ((candidate_sum % (seq_t)cur) != (seq_t)0)
                continue;               /* average not integer */

            seq_t avg = candidate_sum / (seq_t)cur;
            if (!is_triangular(avg))
                continue;               /* average not triangular */

            best_t = t;
            best_k = k;
            break;                      /* found the minimal suitable T_k */
        }

        /* ----  store the new term -------------------------------- */
        ensure_cache_size(cur);
        cache[cur - 1] = best_t;
        ensure_used_size(best_k);
        used_tri[best_k] = true;
        sum_last += best_t;
        cache_len = (size_t)cur;
    }

    return cache[n - 1];
}
