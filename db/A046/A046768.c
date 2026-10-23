/*
 * A046768  Number of partitions of n with equal number of parts congruent to
 *          each of 0, 2 and 3 (mod 4).
 * https://oeis.org/A046768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * ( Sum_{k≥0} x^(9k) / ( ∏_{j=1..k}
 *            (1 - x^(4j))^3 ) ) *
 *            ---------------------------------------------------------------
 *            * ( ∏_{j≥0} (1 - x^(4j+1)) )⁻¹ * * This implementation builds
 *            the generating function by dynamic * programming. It uses three
 *            auxiliary arrays: * p...
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 179, u64 n <= 523,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper to allocate an array of seq_t that works in both C and C++ */
static seq_t *alloc_seq_array(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t *)calloc(len, sizeof(seq_t));
#endif
}

/* Helper to free an array allocated with alloc_seq_array */
static void free_seq_array(seq_t *ptr)
{
#ifdef __cplusplus
    delete[] ptr;
#else
    free(ptr);
#endif
}

static idx_t cache_max = 0;      /* maximum n for which cache_vals is valid */
static seq_t *cache_vals = NULL; /* array of coefficients up to cache_max */

/* Rebuild the cache for all n <= n (inclusive) */
static void rebuild_cache(idx_t n)
{
    size_t N = (size_t)(n);
    size_t sz = N + 1; /* indices 0..N */

    seq_t *p1    = alloc_seq_array(sz);
    seq_t *prod  = alloc_seq_array(sz);
    seq_t *sumG  = alloc_seq_array(sz);
    if (!p1 || !prod || !sumG) return; /* allocation failure */

    /* partitions with parts ≡ 1 (mod 4) */
    p1[0] = 1;
    for (size_t s = 1; s <= N; s += 4)
        for (size_t i = s; i <= N; ++i)
            p1[i] += p1[i - s];

    /* numerator: Sum_{k≥0} x^(9k) / ∏_{j=1..k} (1 - x^(4j))^3 */
    prod[0] = 1;
    sumG[0] = 1; /* k = 0 term */

    size_t Kmax = n / 9; /* largest k with 9k ≤ n */
    for (size_t k = 1; k <= Kmax; ++k) {
        size_t step = 4 * k;   /* 4j step for this k */
        /* multiply prod by (1 - x^step)^(-3) */
        for (int pass = 0; pass < 3; ++pass)
            for (size_t i = step; i <= N; ++i)
                prod[i] += prod[i - step];
        size_t offset = 9 * k;
        for (size_t i = 0; i + offset <= N; ++i)
            sumG[i + offset] += prod[i];
    }

    /* convolution: (sumG) * p1 gives the desired coefficients */
    seq_t *new_vals = alloc_seq_array(sz);
    if (!new_vals) {
        free_seq_array(p1); free_seq_array(prod); free_seq_array(sumG);
        return;
    }

    for (size_t nd = 0; nd <= N; ++nd) {
        seq_t acc = 0;
        for (size_t m = 0; m <= nd; ++m)
            acc += sumG[m] * p1[nd - m];
        new_vals[nd] = acc;
    }

    /* replace old cache */
    if (cache_vals) free_seq_array(cache_vals);
    cache_vals = new_vals;
    cache_max  = n;

    /* free temporary arrays */
    free_seq_array(p1);
    free_seq_array(prod);
    free_seq_array(sumG);
}

seq_t a(idx_t n)
{
    /* Ensure cache is built at least for n = 0 */
    if (!cache_vals) rebuild_cache(0);

    if (n > cache_max) rebuild_cache(n);
    return cache_vals[n];
}
