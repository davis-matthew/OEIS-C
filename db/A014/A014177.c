/*
 * A014177  Inverse of 168th cyclotomic polynomial.
 * https://oeis.org/A014177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the series expansion of
 *            1/Φ168(x), where Φ168 is the 168‑th cyclotomic polynomial. The
 *            sequence is periodic with period 168
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* helpers ------------------------------------------------- */
static void poly_mul(seq_t *res, const seq_t *a, size_t da,
                     const seq_t *b, size_t db)
{
    size_t i, j;
    for (i = 0; i <= da + db; ++i) res[i] = 0;
    for (i = 0; i <= da; ++i) if (a[i]) {
        for (j = 0; j <= db; ++j) if (b[j]) {
            res[i + j] += a[i] * b[j];
        }
    }
}

/* integer division a(x) / b(x) where leading coefficient of b is 1. */
static void poly_div(seq_t *q, size_t *dq,
                     const seq_t *a, size_t na,
                     const seq_t *b, size_t nb)
{
    seq_t tmp[200];
    size_t i, j;
    for (i = 0; i <= na; ++i) tmp[i] = a[i];

    size_t qdeg = na - nb;
    *dq = qdeg;
    for (i = 0; i <= qdeg; ++i) q[i] = 0;

    for (int i = (int)na; i >= (int)nb; --i) {
        seq_t coef = tmp[i];
        if (coef == 0) continue;
        q[i - nb] = coef;
        for (j = 0; j <= nb; ++j) {
            tmp[i - nb + j] -= coef * b[j];
        }
    }
}

/* ------------------------------------------------------- */

static seq_t a_cache[168];
static seq_t phi[49];           /* coefficients of Φ168, ascending */
static int   initialized = 0;

static void init_seq(void)
{
    if (initialized) return;

    /* 1. all divisors of 168 */
    int divs[20];
    int num_div = 0;
    for (int d = 1; d <= 168; ++d)
        if (168 % d == 0) divs[num_div++] = d;

    int idx[169];
    for (int i = 0; i < 169; ++i) idx[i] = -1;
    for (int i = 0; i < num_div; ++i) idx[divs[i]] = i;

    /* 2. compute Φd for each divisor in increasing order */
    seq_t polys[20][200];
    size_t degs[20];

    for (int ti = 0; ti < num_div; ++ti) {
        int d = divs[ti];

        /* numerator x^d – 1 */
        seq_t num[200] = {0};
        num[0] = -1;
        num[d] = 1;
        size_t numdeg = d;

        /* denominator: product of polys of proper divisors of d */
        seq_t denom[200] = {0};
        denom[0] = 1;            /* polynomial “1” */
        size_t denomdeg = 0;

        for (int ei = 0; ei < ti; ++ei) {
            int e = divs[ei];
            if (d % e == 0) {
                seq_t tmp[200];
                poly_mul(tmp, denom, denomdeg, polys[ei], degs[ei]);
                for (size_t k = 0; k <= denomdeg + degs[ei]; ++k)
                    denom[k] = tmp[k];
                denomdeg += degs[ei];
            }
        }

        /* Φd = (x^d – 1) / denom */
        seq_t quo[200];
        size_t quo_deg;
        poly_div(quo, &quo_deg, num, numdeg, denom, denomdeg);

        /* store */
        for (size_t k = 0; k <= quo_deg; ++k) polys[ti][k] = quo[k];
        degs[ti] = quo_deg;
    }

    /* 3. obtain coefficients of Φ168 */
    int id168 = idx[168];
    for (size_t k = 0; k <= degs[id168]; ++k) phi[k] = polys[id168][k];

    /* 4. compute the first 168 terms using the recurrence */
    a_cache[0] = 1;
    for (int n = 1; n < 168; ++n) {
        seq_t sum = 0;
        int kmax = (n < 48) ? n : 48;
        for (int k = 1; k <= kmax; ++k) sum += phi[k] * a_cache[n - k];
        a_cache[n] = -sum;
    }

    initialized = 1;
}

seq_t a(idx_t n)
{
    init_seq();
    return a_cache[(size_t)(n % 168)];
}
