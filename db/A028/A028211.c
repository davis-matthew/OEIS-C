/*
 * A028211  Expansion of 1/((1-6x)(1-8x)(1-9x)(1-11x)).
 * https://oeis.org/A028211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 99*a(n-2) + 2^n*(4^(n+1) - 3^(n+1)) a(0) = 1,
 *            a(1) = 34
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

static seq_t *a_cache  = NULL;   /* sequence values */
static size_t a_cap    = 0;
static size_t a_len    = 0;      /* next uncomputed index */

static seq_t *pow2_cache = NULL; /* powers of 2 */
static size_t p2_cap     = 0;
static size_t p2_len     = 0;

static seq_t *pow3_cache = NULL; /* powers of 3 */
static size_t p3_cap     = 0;
static size_t p3_len     = 0;

static seq_t *pow4_cache = NULL; /* powers of 4 */
static size_t p4_cap     = 0;
static size_t p4_len     = 0;

/* Extend the power caches up to `n` (inclusive). */
static void ensure_powers(idx_t n)
{
    /* 2^n */
    if (pow2_cache == NULL) {
        ensure_cache(0, &pow2_cache, &p2_cap);
        pow2_cache[0] = 1; p2_len = 1;
    }
    if (p2_cap <= (size_t)n) ensure_cache((size_t)n, &pow2_cache, &p2_cap);
    for (idx_t i = p2_len; i <= n; ++i)
        pow2_cache[i] = mul_small(pow2_cache[i-1], 2);
    p2_len = n+1;

    /* 3^n and 4^n up to n+1 because recurrence uses n+1 */
    if (pow3_cache == NULL) {
        ensure_cache(0, &pow3_cache, &p3_cap);
        pow3_cache[0] = 1; p3_len = 1;
    }
    if (p3_cap <= (size_t)(n+1)) ensure_cache((size_t)(n+1), &pow3_cache, &p3_cap);
    for (idx_t i = p3_len; i <= (idx_t)(n+1); ++i)
        pow3_cache[i] = mul_small(pow3_cache[i-1], 3);
    p3_len = (size_t)(n+1)+1;

    if (pow4_cache == NULL) {
        ensure_cache(0, &pow4_cache, &p4_cap);
        pow4_cache[0] = 1; p4_len = 1;
    }
    if (p4_cap <= (size_t)(n+1)) ensure_cache((size_t)(n+1), &pow4_cache, &p4_cap);
    for (idx_t i = p4_len; i <= (idx_t)(n+1); ++i)
        pow4_cache[i] = mul_small(pow4_cache[i-1], 4);
    p4_len = (size_t)(n+1)+1;
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 34;

    /* ensure sequence cache can hold n+1 entries */
    if (a_cache == NULL) {
        ensure_cache(0, &a_cache, &a_cap);
        a_cache[0] = 1;
        a_cache[1] = 34;
        a_len    = 2;
    }
    if (a_cap <= (size_t)n) ensure_cache((size_t)n, &a_cache, &a_cap);

    /* precompute needed powers */
    ensure_powers(n);

    /* compute missing terms up to n */
    for (idx_t i = a_len; i <= n; ++i) {
        /* 20*a(i-1) */
        seq_t term1 = mul_small(a_cache[i-1], 20);
        /* -99*a(i-2) */
        term1 -= mul_small(a_cache[i-2], 99);

        /* diff = 4^(i+1) - 3^(i+1) */
        seq_t diff  = pow4_cache[i+1] - pow3_cache[i+1];
        /* term = 2^i * diff */
        seq_t term  = seq_mul(pow2_cache[i], diff);

        term1 += term;
        a_cache[i] = term1;
    }
    a_len = n+1;
    return a_cache[n];
}
