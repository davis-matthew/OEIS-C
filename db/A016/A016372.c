/*
 * A016372  285th cyclotomic polynomial.
 * https://oeis.org/A016372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 285th cyclotomic polynomial
 *            Φ_285(x)
 * Range:     i8 n <= 144, i16 n <= 144, i32 n <= 144, i64 n <= 144,
 *            i128 n <= 144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

typedef struct {
    size_t deg;
    seq_t *coeffs;
} poly;

static poly *poly_new(size_t deg)
{
    poly *p = (poly *)malloc(sizeof(poly));
    p->deg = deg;
    p->coeffs = (seq_t *)calloc(deg + 1, sizeof(seq_t));
    return p;
}

static void poly_free(poly *p)
{
    free(p->coeffs);
    free(p);
}

static poly *poly_mul(const poly *a, const poly *b)
{
    size_t deg = a->deg + b->deg;
    poly *res = poly_new(deg);
    for (size_t i = 0; i <= a->deg; ++i)
        for (size_t j = 0; j <= b->deg; ++j)
            res->coeffs[i + j] += a->coeffs[i] * b->coeffs[j];
    return res;
}

static poly *poly_divide(const poly *numer, const poly *den)
{
    size_t degq = numer->deg - den->deg;
    poly *q = poly_new(degq);
    seq_t *temp = (seq_t *)malloc((numer->deg + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= numer->deg; ++i)
        temp[i] = numer->coeffs[i];

    for (long long i = (long long)numer->deg; i >= (long long)den->deg; --i) {
        seq_t coeff = temp[i];
        size_t qidx = i - den->deg;
        if (coeff != 0) {
            q->coeffs[qidx] = coeff;
            for (size_t j = 0; j <= den->deg; ++j) {
                temp[qidx + j] -= coeff * den->coeffs[j];
            }
        }
    }
    free(temp);
    return q;
}

static poly *poly_cache[286] = {0};
static char poly_computed[286] = {0};

static poly *poly_compute(idx_t n)
{
    if (poly_computed[n])
        return poly_cache[n];

    poly *res;
    if (n == 1) {
        res = poly_new(1);
        res->coeffs[0] = -1;
        res->coeffs[1] = 1;
    } else {
        /* compute product of all Φ_d for proper divisors d | n */
        poly *den = poly_new(0);
        den->coeffs[0] = 1;
        for (idx_t d = 1; d < n; ++d) {
            if (n % d == 0) {
                poly *pd = poly_compute(d);
                poly *tmp = poly_mul(den, pd);
                poly_free(den);
                den = tmp;
            }
        }
        /* numerator x^n - 1 */
        poly *num = poly_new(n);
        num->coeffs[0] = -1;
        num->coeffs[n] = 1;
        res = poly_divide(num, den);
        poly_free(num);
        poly_free(den);
    }
    poly_cache[n] = res;
    poly_computed[n] = 1;
    return res;
}

seq_t a(idx_t n)
{
    static poly *poly285 = NULL;
    if (!poly285) {
        poly285 = poly_compute(285);
    }
    if (n >= (idx_t)(poly285->deg + 1))
        return 0;
    return poly285->coeffs[n];
}
