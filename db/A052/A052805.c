/*
 * A052805  If B is a collection in which there are C(n-1) [Catalan numbers,
 *          A000108] things with n points, a(n) is the number of subsets
 *          without repetition of B with a total of n points.
 * https://oeis.org/A052805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(0)=1, a(n) = (1/n)*∑_{k=1..n} a(n-k)*b(k) where b(k)=∑_{d|k}
 *            (-1)^{k/d+1} * binomial(2*d-2, d-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------------- */
/*  Cache for sequence a(n) and auxiliary b(k) values                    */
/* --------------------------------------------------------------------- */
#ifndef __cplusplus
/* C implementation */
static seq_t *a_cache = NULL;        /* a[0..n]        */
static seq_t *b_cache = NULL;        /* b[1..n] (b[0] unused) */
static size_t cache_cap = 0;         /* next computed index */

static void compute_b_for_idx(idx_t k, seq_t *b_val)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            idx_t d1 = d;
            idx_t d2 = k / d;

            /* divisor d1 */
            seq_t term1 = binomial(2 * d1 - 2, d1 - 1);
            if (((k / d1 + 1) & 1) == 0)
                sum += term1;
            else
                sum -= term1;

            /* divisor d2 if distinct */
            if (d2 != d1) {
                seq_t term2 = binomial(2 * d2 - 2, d2 - 1);
                if (((k / d2 + 1) & 1) == 0)
                    sum += term2;
                else
                    sum -= term2;
            }
        }
    }
    *b_val = sum;
}

static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;                 /* already available */

    size_t new_cap = (size_t)n + 1;
    seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    seq_t *new_b = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    if (!new_a || !new_b)
        abort();

    /* Assign new pointers */
    a_cache = new_a;
    b_cache = new_b;

    /* Initialise new entries */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        if (i == 0) {                  /* a(0)=1, b(0) unused */
            a_cache[0] = (seq_t)1;
            b_cache[0] = (seq_t)0;
            continue;
        }
        compute_b_for_idx((idx_t)i, &b_cache[i]);

        /* compute a[i] = (sum_{k=1..i} a[i-k]*b[k]) / i */
        seq_t sum = 0;
        for (idx_t k = 1; k <= (idx_t)i; ++k)
            sum += a_cache[(idx_t)i - k] * b_cache[k];
        a_cache[i] = sum / (seq_t)(idx_t)i;
    }
    cache_cap = new_cap;
}
#else
/* C++ implementation */
static std::vector<seq_t> a_cache;
static std::vector<seq_t> b_cache;

static seq_t compute_b_for_idx(idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            idx_t d1 = d;
            idx_t d2 = k / d;

            seq_t term1 = binomial(2 * d1 - 2, d1 - 1);
            if (((k / d1 + 1) & 1) == 0)
                sum += term1;
            else
                sum -= term1;

            if (d2 != d1) {
                seq_t term2 = binomial(2 * d2 - 2, d2 - 1);
                if (((k / d2 + 1) & 1) == 0)
                    sum += term2;
                else
                    sum -= term2;
            }
        }
    }
    return sum;
}

static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (a_cache.size() < need) {
        size_t old_size = a_cache.size();
        a_cache.resize(need);
        b_cache.resize(need);

        if (old_size == 0) {            /* initialise a[0] */
            a_cache[0] = (seq_t)1;
            b_cache[0] = (seq_t)0;
            old_size = 1;
        }

        for (size_t i = old_size; i < need; ++i) {
            b_cache[i] = compute_b_for_idx((idx_t)i);

            seq_t sum = 0;
            for (idx_t k = 1; k <= (idx_t)i; ++k)
                sum += a_cache[(idx_t)i - k] * b_cache[k];
            a_cache[i] = sum / (seq_t)(idx_t)i;
        }
    }
}
#endif

/* --------------------------------------------------------------------- */
/*  Public interface                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
#if defined(__cplusplus)
    ensure_cache(n);
    return a_cache[(size_t)n];
#else
    ensure_cache(n);
    return a_cache[(size_t)n];
#endif
}
