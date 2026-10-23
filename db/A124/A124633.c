/*
 * A124633  Poincaré series [or Poincare series] P(C^o_{3,2}; x).
 * https://oeis.org/A124633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124633: Poincaré series
   a(n) = coefficient of x^n in (x^12+3x^10+6x^9+9x^8+6x^7+12x^6+6x^5+9x^4+6x^3+3x^2+1)
/          ((1-x)^2·(1-x^2)^3·(1-x^3)^3·(1-x^4)) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

/* Numerator coefficients: P[n] = coeff of x^n in numerator  (degree 12) */
static const int P[13] = {
    1, 0, 3, 6, 9, 6, 12, 6, 9, 6, 3, 0, 1
};

/* Denominator coefficients will be computed once and stored here */
static int Q[22];          /* Q[0] = 1 */
static int Q_computed = 0;

/* Compute Q(x) coefficients: Q(x) = (1-x)^2·(1-x^2)^3·(1-x^3)^3·(1-x^4) */
static void compute_Q(void)
{
    if (Q_computed) return;

    /* Constituent polynomials */
    const int A[3]  = { 1, -2, 1 };   /* (1-x)^2               */
    const int B[7]  = { 1, 0, -3, 0, 3, 0, -1 };   /* (1-x^2)^3      */
    const int C[10] = { 1, 0, 0, -3, 0, 0, 3, 0, 0, -1 };   /* (1-x^3)^3 */
    const int D[5]  = { 1, 0, 0, 0, -1 };   /* (1-x^4)          */

    int temp1[9]  = {0};
    int temp2[18] = {0};
    int temp3[22] = {0};

    /* A * B -> temp1 */
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 7; ++j)
            temp1[i + j] += A[i] * B[j];

    /* temp1 * C -> temp2 */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 10; ++j)
            temp2[i + j] += temp1[i] * C[j];

    /* temp2 * D -> Q (length 22) */
    for (int i = 0; i < 18; ++i)
        for (int j = 0; j < 5; ++j)
            temp3[i + j] += temp2[i] * D[j];

    /* Copy to global array */
    for (int i = 0; i < 22; ++i)
        Q[i] = temp3[i];

    Q_computed = 1;
}

/* Memoisation cache for a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size in elements */
static idx_t  max_computed = 0;   /* highest n already stored */

/* Grow cache buffer if needed and compute new terms */
static void ensure_cache(idx_t needed)
{
    if (needed <= max_computed) return;

    if (cache_cap < (size_t)(needed + 1)) {
        size_t new_cap = (needed + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    compute_Q();   /* ensure Q[ ] is ready */

    for (idx_t i = max_computed + 1; i <= needed; ++i) {
        seq_t rhs = (seq_t)( (i <= 12) ? P[i] : 0 );

        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        idx_t lim = (i < 21) ? i : 21;
        for (idx_t k = 1; k <= lim; ++k) {
            int qk = Q[k];
            seq_t term = mul_small(cache[i - k], (unsigned int)(qk < 0 ? -qk : qk));
            if (qk < 0)
                pos_sum += term;
            else
                neg_sum += term;
        }

        rhs += pos_sum;
        rhs -= neg_sum;
        cache[i] = rhs;
    }

    max_computed = needed;
}

/* Public interface -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)P[0];   /* a(0) = 1 */
        max_computed = 0;
    }
    ensure_cache(n);
    return cache[n];
}
