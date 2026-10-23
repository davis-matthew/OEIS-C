/*
 * A127811  A127790(n)/2.
 * https://oeis.org/A127811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence derived from *
 *            (1-x)^10*(1+x)^2*(x^2+x+1)^4 = 1 + q1 x + q2 x^2 + ... + q20
 *            x^20. * With a(0)=0, a(1)=1. For n > 0: * a(n) = - (q1 a(n-1)+q2
 *            a(n-2)+…+q20 a(n-20)) + (n==1?1:0)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* coefficients of the recurrence, qk is the coefficient of x^k in
   (1-x)^10*(1+x)^2*(x^2+x+1)^4, stored as int32_t */
static int32_t q[21] = {0};
static bool q_initialized = false;

/* multiplication of two small integer polynomials
 * a[0..deg_a], b[0..deg_b]  -> out[0..deg_a+deg_b]   */
static void poly_mul(const int32_t *a, int deg_a,
                     const int32_t *b, int deg_b,
                     int32_t *out)
{
    for (int i = 0; i <= deg_a; ++i)
        if (a[i])
            for (int j = 0; j <= deg_b; ++j)
                if (b[j])
                    out[i + j] += a[i] * b[j];
}

/* compute the recurrence coefficients once */
static void init_coef(void)
{
    if (q_initialized)
        return;

    /* (1 - x)^10 : binomial with alternating signs */
    int32_t p1[21] = {0};
    for (int k = 0; k <= 10; ++k) {
        seq_t c = binom(10, k);            /* binomial coefficient */
        int32_t v = (int32_t)c;            /* small enough for int32 */
        if (k & 1)
            v = -v;
        p1[k] = v;
    }

    /* (1 + x)^2 = 1 + 2x + x^2 */
    int32_t a12[3] = {1, 2, 1};
    int32_t p2[21] = {0};
    poly_mul(p1, 10, a12, 2, p2);           /* degree 12 */

    /* (x^2 + x + 1)^2 = degree 4  */
    int32_t base[3] = {1, 1, 1};
    int32_t base2[21] = {0};
    poly_mul(base, 2, base, 2, base2);

    /* (x^2 + x + 1)^4 = ( (x^2+x+1)^2 )^2  -> degree 8 */
    int32_t base4[21] = {0};
    poly_mul(base2, 4, base2, 4, base4);

    /* final product: (1-x)^10 * (1+x)^2 * (x^2+x+1)^4   */
    poly_mul(p2, 12, base4, 8, q);           /* degree 20 */

    q_initialized = true;
}

/* cache of computed sequence values */
static seq_t *cache   = NULL;   /* linear array of a(n) */
static size_t  cap    = 0;      /* capacity of cache */
static idx_t   last   = 0;      /* largest index computed */

/* ensure cache has space up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cap = (n >= 20 ? n + 1 : 21);
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = 0;           /* a(0) = 0 */
    } else if ((idx_t)cap <= n) {
        size_t newcap = cap;
        while ((idx_t)newcap <= n)
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        cap = newcap;
    }
}

/* a(n) : OEIS A127790(n)/2 */
seq_t a(idx_t n)
{
    init_coef();
    ensure_cache(n);

    if (n <= last)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t rhs = (i == 1) ? 1 : 0;
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        for (int k = 1; k <= 20; ++k) {
            if (i < (idx_t)k)
                continue;
            int32_t ck = q[k];
            if (ck >= 0)
                sum_pos += (seq_t)ck * cache[i - k];
            else
                sum_neg += (seq_t)(-ck) * cache[i - k];
        }
        cache[i] = rhs + sum_neg - sum_pos;   /* guaranteed non‑negative */
    }
    last = n;
    return cache[n];
}
