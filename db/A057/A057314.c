/*
 * A057314  Coordination sequence T1 for Zeolite Code SAV.
 * https://oeis.org/A057314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) : Coordination sequence T1 for Zeolite Code SAV
 * (OEIS A057314).  The generating function is
 *    (1 + x) * (1 + x + 3x² + 4x³ + 6x⁴ + 8x⁵ + 9x⁶ + 9x⁷ + 9x⁸ + 9x⁹ + 6x¹⁰ + 4x¹¹ + 3x¹² + 2x¹³ + x¹⁵ – x¹⁶)
 *   --------------------------------------------------------------
 *   ((1 – x)³ * (1 – x + x²) * (1 + x²) * (1 + x + x²) * (1 + x + x² + x³ + x⁴ + x⁵ + x⁶))
 *
 * A linear recurrence of order 15 is obtained from the denominator.
 * The numerator coefficients (after multiplying by 1+x) are
 *  [1, 2, 4, 7, 10, 14, 17, 18, 18, 18, 15, 10, 7, 5, 2, 1, 0, -1].
 *
 * The recurrence (for n ≥ 15) is
 *   a(n) = -∑_{i=1}^{15} c_i · a(n–i) + p_n
 * where c_i are the denominator coefficients (with c_0 = 1) and
 * p_n is the n‑th numerator coefficient (zero for n > 17).
 */

#include <stddef.h>
#include <stdlib.h>

/* ---------- Coefficient calculation (executed once) ---------- */

#define DEN_ORDER   15               /* denominator degree   */
#define NUM_DEG     17               /* numerator degree after multiplying by (1+x) */
#define NUM_COEFF   (NUM_DEG + 1)    /* number of numerator coefficients */

static int32_t den_coeff[DEN_ORDER + 1] = {0};
static int32_t num_coeff[NUM_COEFF]     = {0};
static int     coeffs_built = 0;

static void build_coeffs(void)
{
    /* ----- Build denominator polynomial ---------- */
    int cur[25] = {0};
    int deg = 0;
    cur[0] = 1;                       /* start with 1 */

    /* (1 - x) ^ 3 */
    int f1[2] = {1, -1};
    for (int t = 0; t < 3; ++t) {
        int tmp[25] = {0};
        for (int i = 0; i <= deg; ++i)
            for (int j = 0; j < 2; ++j)
                tmp[i + j] += cur[i] * f1[j];
        ++deg;
        for (int i = 0; i <= deg; ++i) cur[i] = tmp[i];
    }

    /* (1 - x + x^2) */
    int f2[3] = {1, -1, 1};
    {
        int tmp[25] = {0};
        for (int i = 0; i <= deg; ++i)
            for (int j = 0; j < 3; ++j)
                tmp[i + j] += cur[i] * f2[j];
        deg += 2;
        for (int i = 0; i <= deg; ++i) cur[i] = tmp[i];
    }

    /* (1 + x^2) */
    int f3[3] = {1, 0, 1};
    {
        int tmp[25] = {0};
        for (int i = 0; i <= deg; ++i)
            for (int j = 0; j < 3; ++j)
                tmp[i + j] += cur[i] * f3[j];
        deg += 2;
        for (int i = 0; i <= deg; ++i) cur[i] = tmp[i];
    }

    /* (1 + x + x^2) */
    int f4[3] = {1, 1, 1};
    {
        int tmp[25] = {0};
        for (int i = 0; i <= deg; ++i)
            for (int j = 0; j < 3; ++j)
                tmp[i + j] += cur[i] * f4[j];
        deg += 2;
        for (int i = 0; i <= deg; ++i) cur[i] = tmp[i];
    }

    /* (1 + x + x^2 + x^3 + x^4 + x^5 + x^6) */
    int f5[7] = {1, 1, 1, 1, 1, 1, 1};
    {
        int tmp[25] = {0};
        for (int i = 0; i <= deg; ++i)
            for (int j = 0; j < 7; ++j)
                tmp[i + j] += cur[i] * f5[j];
        deg += 6;
        for (int i = 0; i <= deg; ++i) cur[i] = tmp[i];
    }

    /* Store denominator coefficients (c_0 = 1, c_1 … c_15) */
    for (int i = 0; i <= deg; ++i) den_coeff[i] = cur[i];
    for (int i = deg + 1; i <= DEN_ORDER; ++i) den_coeff[i] = 0;

    /* ----- Build numerator coefficients (p_k) ---------- */
    /* Base polynomial N(x) (before multiplying by (1+x)) */
    int32_t N[18] = {0};   /* indices 0 … 16 */
    N[0]  =  1;
    N[1]  =  1;
    N[2]  =  3;
    N[3]  =  4;
    N[4]  =  6;
    N[5]  =  8;
    N[6]  =  9;
    N[7]  =  9;
    N[8]  =  9;
    N[9]  =  9;
    N[10] =  6;
    N[11] =  4;
    N[12] =  3;
    N[13] =  2;
    N[14] =  0;
    N[15] =  1;
    N[16] = -1;

    for (int k = 0; k < NUM_COEFF; ++k) {
        int val = 0;
        if (k <= 16) val += N[k];
        if (k - 1 >= 0 && k - 1 <= 16) val += N[k - 1];
        num_coeff[k] = val;
    }

    coeffs_built = 1;
}

/* ---------- Cache for computed terms ---------- */

static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;
static idx_t  cache_cap  = 0;

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Build coefficients and initialize cache on first call */
    if (!coeffs_built) {
        build_coeffs();
        cache_cap = 128;                      /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {

        /* Accumulate contributions with positive and negative signs separately */
        seq_t pos = 0;
        seq_t neg = 0;

        /* Recurrence part */
        int max_i = (k < (idx_t)DEN_ORDER) ? (int)k : DEN_ORDER;
        for (int i = 1; i <= max_i; ++i) {
            int coeff = den_coeff[i];
            if (coeff == 0) continue;

            seq_t mult = (seq_t)((coeff > 0 ? coeff : -coeff)) * cache[k - i];
            if (coeff < 0)
                pos += mult;          /* subtracting a negative term => add */
            else
                neg += mult;          /* subtracting a positive term => subtract */
        }

        /* Numerator term */
        if (k < NUM_COEFF) {
            int nk = num_coeff[k];
            if (nk >= 0)
                pos += (seq_t)nk;
            else
                neg += (seq_t)(-nk);
        }

        /* Final value: pos – neg (pos must be ≥ neg) */
        cache[k] = pos - neg;
    }

    cache_len = n + 1;
    return cache[n];
}
