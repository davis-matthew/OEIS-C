/*
 * A124612  Poincaré series [or Poincare series] P(C_{4,2}; x).
 * https://oeis.org/A124612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Poincaré series for C_{4,2}:  a(n) = N_n - Σ_{j=1}^{degD} d_j * a(n-j)
   (d_0 = 1, N_n = 0 for n > degN). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

/* Denominator coefficients (D(x)) */
static int degD = 0;
static int d_coeff[50];        /* sufficient for degree 46 */

/* Numerator coefficients (N(x)) */
static int degN = 0;
static int64_t n_coeff[20];

/* Cache for computed a(n) */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static bool inited = false;

/* Construct the polynomial coefficients once. */
static void init_polynomials(void)
{
    if (inited) return;
    inited = true;

    /* (1 - x)^3 = 1 -3x +3x^2 -x^3 */
    int64_t p0[4] = {1, -3, 3, -1};

    /* (1 - x^2)^4 = 1 -4x^2 +6x^4 -4x^6 +x^8 */
    int64_t p1[9] = {0};
    p1[0] = 1;
    p1[2] = -4;
    p1[4] =  6;
    p1[6] = -4;
    p1[8] =  1;

    /* (1 - x^3)^5 = 1 -5x^3 +10x^6 -10x^9 +5x^12 -x^15 */
    int64_t p2[16] = {0};
    p2[0]  = 1;
    p2[3]  = -5;
    p2[6]  = 10;
    p2[9]  = -10;
    p2[12] = 5;
    p2[15] = -1;

    /* (1 - x^4)^5 = 1 -5x^4 +10x^8 -10x^12 +5x^16 -x^20 */
    int64_t p3[21] = {0};
    p3[0]  = 1;
    p3[4]  = -5;
    p3[8]  = 10;
    p3[12] = -10;
    p3[16] = 5;
    p3[20] = -1;

    /* Multiply polynomials: tmp1 = p0 * p1, tmp2 = tmp1 * p2, d_coeff = tmp2 * p3 */
    int64_t tmp1[20] = {0};
    int64_t tmp2[30] = {0};
    int64_t tmp3[50] = {0};

    size_t l1 = 4, l2 = 9;
    for (size_t i = 0; i < l1 + l2 - 1; ++i) tmp1[i] = 0;
    for (size_t i = 0; i < l1; ++i)
        for (size_t j = 0; j < l2; ++j)
            tmp1[i + j] += p0[i] * p1[j];

    size_t l3 = l1 + l2 - 1, l4 = 16;
    for (size_t i = 0; i < l3 + l4 - 1; ++i) tmp2[i] = 0;
    for (size_t i = 0; i < l3; ++i)
        for (size_t j = 0; j < l4; ++j)
            tmp2[i + j] += tmp1[i] * p2[j];

    size_t l5 = l3 + l4 - 1, l6 = 21;
    for (size_t i = 0; i < l5 + l6 - 1; ++i) tmp3[i] = 0;
    for (size_t i = 0; i < l5; ++i)
        for (size_t j = 0; j < l6; ++j)
            tmp3[i + j] += tmp2[i] * p3[j];

    /* Store coefficients */
    for (size_t i = 0; i < l5 + l6 - 1; ++i) {
        d_coeff[i] = (int)tmp3[i];
    }
    degD = (int)(l5 + l6 - 2);   /* last degree with non‑zero coefficient */

    /* Numerator: (1 - x^2 + x^4) and the second polynomial */
    int64_t n1[5] = {0};
    n1[0] = 1;
    n1[2] = -1;
    n1[4] = 1;

    int64_t n2[11] = {0};
    n2[0]  = 1;
    n2[1]  = -1;
    n2[3]  = -1;
    n2[4]  = 1;
    n2[5]  = 2;
    n2[6]  = 1;
    n2[7]  = -1;
    n2[9]  = -1;
    n2[10] = 1;

    int64_t tn[15] = {0};
    size_t fun = 5, fvn = 11;
    for (size_t i = 0; i < fun + fvn - 1; ++i) tn[i] = 0;
    for (size_t i = 0; i < fun; ++i)
        for (size_t j = 0; j < fvn; ++j)
            tn[i + j] += n1[i] * n2[j];

    for (size_t i = 0; i < fun + fvn - 1; ++i) {
        n_coeff[i] = tn[i];
    }
    degN = (int)(fun + fvn - 2);
}

/* Ensure that terms up to index n are computed. */
static void ensure_terms(idx_t n)
{
    if (!inited) init_polynomials();

    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_size; i < need; ++i) {
        seq_t val = (i <= (size_t)degN) ? (seq_t)n_coeff[i] : 0;

        for (size_t j = 1; j <= (size_t)degD && j <= i; ++j) {
            if (d_coeff[j] == 0) continue;
            unsigned comp = (unsigned)(d_coeff[j] < 0 ? -d_coeff[j] : d_coeff[j]);
            seq_t prod   = mul_small(cache[i - j], comp);
            if (d_coeff[j] > 0)
                val -= prod;
            else
                val += prod;
        }
        cache[i] = val;
    }
    cache_size = need;
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[(size_t)n];
}
