/*
 * A014966  Inverse of 957th cyclotomic polynomial.
 * https://oeis.org/A014966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the n‑th coefficient of the power‑series expansion of
 *            1/Φ_957(x), where Φ_957 is the 957th cyclotomic polynomial. The
 *            sequence is periodic with period 957 and satisfies the linear
 *            recurrence a(0)=1, a(n) = -∑_{i=1}^{min(560,n)} Φ_957[i] ·
 *            a(n−i)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Poly ---------------------------------------------------------------------*/
typedef struct {
    seq_t *c;      /* coefficients: c[i] is coefficient of x^i */
    size_t len;    /* number of coefficients (degree+1) */
} Poly;

/* Multiply two polynomials */
static Poly poly_mul(const Poly *a, const Poly *b)
{
    Poly p;
    p.len = a->len + b->len - 1;
    p.c   = (seq_t*)calloc(p.len, sizeof(seq_t));
    for (size_t i = 0; i < a->len; ++i) {
        if (a->c[i] == (seq_t)0) continue;
        for (size_t j = 0; j < b->len; ++j) {
            if (b->c[j] == (seq_t)0) continue;
            p.c[i + j] += a->c[i] * b->c[j];
        }
    }
    return p;
}

/* Exact polynomial division num / den (den is monic). Assumes exact division. */
static Poly poly_div(const Poly *num, const Poly *den)
{
    if (den->len == 1) {                 /* divisor is 1 */
        Poly q;
        q.c   = (seq_t*)malloc(num->len * sizeof(seq_t));
        q.len = num->len;
        for (size_t i = 0; i < num->len; ++i) q.c[i] = num->c[i];
        return q;
    }

    const size_t nd = num->len - den->len + 1;
    Poly q;
    q.c   = (seq_t*)calloc(nd, sizeof(seq_t));
    q.len = nd;

    /* mutable copy of numerator */
    seq_t *r = (seq_t*)calloc(num->len, sizeof(seq_t));
    for (size_t i = 0; i < num->len; ++i) r[i] = num->c[i];

    for (int k = (int)num->len - 1; k >= (int)den->len - 1; --k) {
        seq_t coef = r[k];
        if (coef == (seq_t)0) continue;
        int shift = k - (int)den->len + 1;   /* shift for this step       */
        q.c[shift] = coef;                    /* leading coeff of divisor is 1 */
        for (size_t j = 0; j < den->len; ++j) {
            r[shift + j] -= coef * den->c[j];
        }
    }
    free(r);
    return q;
}

/* Polynomial x^k - 1 */
static Poly poly_pow_minus_one(size_t k)
{
    Poly p;
    p.len = k + 1;
    p.c   = (seq_t*)calloc(p.len, sizeof(seq_t));
    p.c[0] = -(seq_t)1;       /* constant term -1 */
    p.c[k] = (seq_t)1;        /* leading term +1 */
    return p;
}

/* Free polynomial memory */
static void poly_free(Poly *p)
{
    if (p->c) free(p->c);
    p->c   = NULL;
    p->len = 0;
}

/* --------------------------------------------------------------------------*/
static seq_t pattern[957];
static bool   initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Positive factors: (x^k−1) with μ(d)=+1 */
        static const size_t pos_k[] = {957, 29, 11, 3};

        /* Negative factors: (x^k−1) with μ(d)=−1 */
        static const size_t neg_k[] = {319, 87, 33, 1};

        /* Build the product of positive factors */
        Poly pos = {.c = (seq_t*)calloc(1, sizeof(seq_t)), .len = 1};
        pos.c[0] = (seq_t)1;
        for (size_t i = 0; i < sizeof(pos_k)/sizeof(pos_k[0]); ++i) {
            Poly fac = poly_pow_minus_one(pos_k[i]);
            Poly tmp = poly_mul(&pos, &fac);
            poly_free(&pos);
            poly_free(&fac);
            pos = tmp;
        }

        /* Build the product of negative factors */
        Poly neg = {.c = (seq_t*)calloc(1, sizeof(seq_t)), .len = 1};
        neg.c[0] = (seq_t)1;
        for (size_t i = 0; i < sizeof(neg_k)/sizeof(neg_k[0]); ++i) {
            Poly fac = poly_pow_minus_one(neg_k[i]);
            Poly tmp = poly_mul(&neg, &fac);
            poly_free(&neg);
            poly_free(&fac);
            neg = tmp;
        }

        /* Φ_957(x) = pos / neg  */
        Poly phi = poly_div(&pos, &neg);
        poly_free(&pos);
        poly_free(&neg);

        /* Generate the first 957 terms via the linear recurrence */
        pattern[0] = (seq_t)1;
        size_t deg = phi.len - 1;          /* degree of Φ_957 */
        for (size_t m = 1; m < 957; ++m) {
            seq_t sum = (seq_t)0;
            size_t imax = (m < deg) ? m : deg;
            for (size_t i = 1; i <= imax; ++i) {
                if (phi.c[i] != (seq_t)0)
                    sum += phi.c[i] * pattern[m - i];
            }
            pattern[m] = -sum;
        }

        poly_free(&phi);
        initialized = true;
    }

    return pattern[(size_t)(n % 957ull)];
}
