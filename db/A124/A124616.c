/*
 * A124616  Poincaré series [or Poincare series] P(T_{4,2}; x).
 * https://oeis.org/A124616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * (1 - x + x^3 + x^5 - x^7 + x^8) /
 *            * ((1 - x)^5 (1 - x^2)^4 (1 - x^3)^5 (1 - x^4)^3)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of the reciprocal of the denominator polynomial (1 - x)^5
 * (1 - x^2)^4 (1 - x^3)^5 (1 - x^4)^3  */
static seq_t *den_inv = NULL;
/* cache of the final sequence coefficients a(0..max_n) */
static seq_t *coeff = NULL;
/* current maximum index that has been computed (0‑based) */
static size_t cache_len = 0;

/* Recompute the cache up to index N (inclusive) */
static void recompute(idx_t N)
{
    size_t size = (size_t)N + 1;

    /* Allocate or grow arrays */
    den_inv = (seq_t *)realloc(den_inv, size * sizeof(seq_t));
    coeff   = (seq_t *)realloc(coeff,   size * sizeof(seq_t));

    /* ---- 1. Compute 1/D(x)  ---- */
    for (size_t i = 0; i < size; ++i)
        den_inv[i] = 0;
    den_inv[0] = 1;

    /* factors (k, exponent) of the denominator */
    const struct { size_t k; size_t e; } facs[] = {
        {1, 5},
        {2, 4},
        {3, 5},
        {4, 3}
    };
    const size_t fac_cnt = sizeof(facs) / sizeof(facs[0]);

    for (size_t fi = 0; fi < fac_cnt; ++fi) {
        size_t k = facs[fi].k;
        size_t e = facs[fi].e;
        /* Convolve e times with 1/(1 - x^k) = 1 + x^k + x^{2k} + …   */
        for (size_t t = 0; t < e; ++t) {
            for (size_t i = k; i < size; ++i) {
                den_inv[i] += den_inv[i - k];
            }
        }
    }

    /* ---- 2. Compute a(n) by multiplying with numerator  ---- */
    for (size_t i = 0; i < size; ++i) {
        /* pos = B[i] + B[i-3] + B[i-5] + B[i-8] */
        seq_t pos = den_inv[i];
        if (i >= 3) pos += den_inv[i - 3];
        if (i >= 5) pos += den_inv[i - 5];
        if (i >= 8) pos += den_inv[i - 8];

        /* neg = B[i-1] + B[i-7] */
        seq_t neg = 0;
        if (i >= 1) neg += den_inv[i - 1];
        if (i >= 7) neg += den_inv[i - 7];

        /* Since the final a(i) is non‑negative, pos ≥ neg for all i */
        coeff[i] = pos - neg;
    }

    cache_len = size;
}

/* Ensure that cache contains values up to n */
static void ensure(idx_t n)
{
    if ((size_t)n + 1 > cache_len)
        recompute(n);
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    ensure(n);
    return coeff[(size_t)n];
}
