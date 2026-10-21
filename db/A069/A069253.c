/*
 * A069253  Number of basis partitions of n+100 with Durfee square size 10.
 * https://oeis.org/A069253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 118, u64 n <= 1421,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069253 – Number of basis partitions of n+100 with Durfee square size 10
 *
 *  The sequence satisfies a linear recurrence of order 36 with constant
 *  coefficients derived from the rational generating function
 *
 *      A(x) = P(x) / Q(x)
 *
 *  where   Q(x) = (1-x)^10 (x^2+x+1)^3 (x^4+x^3+x^2+x+1)^2 (x^6+x^3+1)
 *                               (x^6+x^5+x^4+x^3+x^2+x+1)
 *
 *  and
 *
 *      P(x) = (x^2-x+1)(x^2+1)(x^4-x^2+1)(x^6-x^5+x^4-x^3+x^2-x+1)
 *           (x^6-x^3+1)(x^8-x^6+x^4-x^2+1)(x^8+1)
 *
 *  For all n ≥ 0
 *
 *      a[n] =  (sum of positive terms) – (sum of negative terms)
 *
 *  where the positive/negative parts are accumulated separately using
 *  the recurrence
 *
 *      a[n] = p[n] – Σ_{k=1}^{36} q[k]·a[n‑k]        (q[0] = 1)
 *
 *  The coefficients q[k] and p[n] are the integer coefficients of Q(x)
 *  and P(x).  They are computed once at program start by direct
 *  polynomial multiplication.
 *
 *  The implementation keeps a static cache so that a(n) can be queried
 *  in arbitrary order in O(n·order) time with a very small constant.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_poly.h"

/* local coefficient storage */
static int64_t q_coeff[37];
static int64_t p_coeff[37];
static bool coeff_ready = false;

/* cache of computed terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/*--- building the coefficient arrays -----------------------------------*/
static void build_coeffs(void)
{
    if (coeff_ready)
        return;

    /* denominator factors */
    static const int64_t fac1[11] = {1,-10,45,-120,210,-252,210,-120,45,-10,1};   /* (1-x)^10 */
    static const int64_t fac2[7]  = {1,3,6,7,6,3,1};                               /* (x^2+x+1)^3  */
    static const int64_t fac3[5]  = {1,1,1,1,1};                                   /* (x^4+…+1)^2  */
    static const int64_t fac4[7]  = {1,0,0,1,0,0,1};                               /* (x^6+x^3+1)  */
    static const int64_t fac5[7]  = {1,1,1,1,1,1,1};                               /* (x^6+…+1)   */

    int64_t cur[200] = {0};
    int curdeg = 0;
    cur[0] = 1;                       /* start with 1 */

    mul_poly(cur, &curdeg, fac1, 10);
    mul_poly(cur, &curdeg, fac2, 6);
    mul_poly(cur, &curdeg, fac3, 4);   /* first copy of (1+x+…+x^4) */
    mul_poly(cur, &curdeg, fac3, 4);   /* second copy (square)   */
    mul_poly(cur, &curdeg, fac4, 6);
    mul_poly(cur, &curdeg, fac5, 6);

    for (int i = 0; i <= 36; ++i)
        q_coeff[i] = (i <= curdeg) ? cur[i] : 0;

    /* numerator factors */
    static const int64_t num1[3] = {1,-1,1};                        /* (x^2-x+1)   */
    static const int64_t num2[3] = {1,0,1};                         /* (x^2+1)     */
    static const int64_t num3[5] = {1,0,-1,0,1};                    /* (x^4-x^2+1) */
    static const int64_t num4[7] = {1,-1,1,-1,1,-1,1};             /* (x^6-…+1)   */
    static const int64_t num5[7] = {1,0,0,-1,0,0,1};              /* (x^6-x^3+1) */
    static const int64_t num6[9] = {1,0,-1,0,1,0,-1,0,1};        /* (x^8-…+1)   */
    static const int64_t num7[9] = {1,0,0,0,0,0,0,0,1};           /* (x^8+1)     */

    int64_t cur2[200] = {0};
    int cur2deg = 0;
    cur2[0] = 1;                       /* start with 1 */

    mul_poly(cur2, &cur2deg, num1, 2);
    mul_poly(cur2, &cur2deg, num2, 2);
    mul_poly(cur2, &cur2deg, num3, 4);
    mul_poly(cur2, &cur2deg, num4, 6);
    mul_poly(cur2, &cur2deg, num5, 6);
    mul_poly(cur2, &cur2deg, num6, 8);
    mul_poly(cur2, &cur2deg, num7, 8);

    for (int i = 0; i <= 36; ++i)
        p_coeff[i] = (i <= cur2deg) ? cur2[i] : 0;

    coeff_ready = true;
}

/*--- expand the cache up to index n ----------------------------------*/
static void compute_up_to(idx_t n)
{
    build_coeffs();

    while (cache_len <= n) {
        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t i = cache_len;
        seq_t pos = 0, neg = 0;

        /* contribution from numerator coefficient */
        if (i <= 36) {
            int64_t pi = p_coeff[i];
            if (pi >= 0)
                pos += (seq_t)pi;
            else
                neg += (seq_t)(-pi);
        }

        /* contributions from previous terms via denominator coefficients */
        for (int j = 1; j <= 36 && (size_t)j <= i; ++j) {
            int64_t qj = q_coeff[j];
            if (qj == 0) continue;
            seq_t prod = cache[i - j] * (seq_t)(qj > 0 ? qj : -qj);
            if (qj > 0)
                neg += prod;
            else
                pos += prod;
        }

        cache[i] = pos - neg;   /* pos ≥ neg by the nature of the recurrence */
        ++cache_len;
    }
}

/*--- public function required by the framework -----------------------*/
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}
