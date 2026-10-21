/*
 * A086622  G.f. A(x) satisfies: A(x) = 1/(1-2*x) + x^2*A(x)^2.
 * https://oeis.org/A086622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   binomial, catalan, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086622: a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k,k) * Catalan(k) * 2^(n-2k) */

/* 
 * The function uses two implementations:
 * 1. For integer seq_t types (≤ 128 bits) the sum formula is computed
 *    using a 128‑bit intermediate to avoid overflow.  This covers the
 *    64‑bit and 128‑bit builds, where the sequence values are only
 *    required up to the point that they fit in the target size.
 * 2. For arbitrary‑precision seq_t types the recurrence
 *    (n+2)a(n) + 4(−n−1)a(n−1) + 4a(n−2) + 4(2n−3)a(n−3) = 0
 *    is used.  All arithmetic is performed with seq_t operators,
 *    which are overloaded for the big integer class.
 */

#include <stdlib.h>
#include <stdint.h>

/* The following helpers are provided by the build environment */
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* Optional inclusion of seq_mul for arbitrary‑precision builds.
 * It is not needed in this code, but the header guard prevents a
 * compilation error when the function is absent. */
#if __has_include("../utils/seq_mul.h")
#include "../utils/seq_mul.h"
#endif

/* ===========================================================================
 * Caching infrastructure
 * =========================================================================== */

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t  cap   = 0;             /* allocated size */
static idx_t  filled = 0;            /* number of terms stored (next index to fill) */

/* Ensure cache can hold terms up to idx 'n' */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation */
        cap = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return;          /* out of memory – should not happen */
        /* fill first three terms explicitly for recurrence branch */
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)2;
        if (cap > 2) cache[2] = (seq_t)5;
        filled = 3;
    }
    if (n + 1 <= cap) return;
    /* grow cache */
    while (n + 1 > cap) cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) return;
    cache = new_cache;
}

/* ===========================================================================
 * Main sequence function
 * =========================================================================== */

seq_t a(idx_t n)
{
    /* Fast path – return from cache if already computed */
    if (cache != NULL && n < filled)
        return cache[n];

    /* Ensure cache exists */
    ensure_cache(n);

    /* ---------- integer seq_t implementation --------------------------------- */
    if (sizeof(seq_t) <= 8) {
        /* Use sum formula with 128‑bit intermediate */
        for (idx_t i = filled; i <= n; ++i) {
            idx_t half = i / 2;
            __uint128_t sum = 0;
            for (idx_t k = 0; k <= half; ++k) {
                seq_t bin = binomial(i - k, k);
                seq_t cat = catalan(k);

                /* 2^(i-2k) using 128‑bit shift */
                idx_t exp = i - 2 * k;
                __uint128_t pow2 = (__uint128_t)1 << (unsigned int)exp;

                /* intermediate multiplication in 128‑bit */
                __uint128_t temp = (__uint128_t)bin * (__uint128_t)cat;
                temp = temp * pow2;

                sum += temp;
            }
            cache[i] = (seq_t)sum;  /* sum fits in seq_t for the tested range */
        }
        filled = n + 1;
        return cache[n];
    }

    /* ---------- arbitrary‑precision seq_t implementation --------------------- */
    /* Recurrence: (n+2)a(n) + 4(-n-1)a(n-1) + 4a(n-2) + 4(2n-3)a(n-3) = 0 */
    for (idx_t i = filled; i <= n; ++i) {
        if (i == 0) { cache[i] = (seq_t)1; continue; }
        if (i == 1) { cache[i] = (seq_t)2; continue; }
        if (i == 2) { cache[i] = (seq_t)5; continue; }

        /* term1 = 4*(i+1)*a(i-1) */
        seq_t factor1 = (seq_t)(4 * ((seq_t)(i + 1)));
        seq_t term1 = factor1 * cache[i - 1];

        /* term2 = 4 * a(i-2) */
        seq_t term2 = (seq_t)4 * cache[i - 2];

        /* term3 = 4*(2*i-3)*a(i-3) */
        seq_t factor3 = (seq_t)(4 * ((seq_t)(2 * i - 3)));
        seq_t term3 = factor3 * cache[i - 3];

        /* numerator = term1 - term2 - term3 */
        seq_t numerator = term1 - term2 - term3;

        /* denominator = i + 2 */
        seq_t denominator = (seq_t)(i + 2);

        cache[i] = numerator / denominator;  /* division is exact */
    }
    filled = n + 1;
    return cache[n];
}
