/*
 * A015153  Inverse of 1144th cyclotomic polynomial.
 * https://oeis.org/A015153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_1144(x); period 1144,
 *            recurrence: a(0)=1, a(n)= -∑_{k=1}^{min(n,deg)} φ_k * a(n-k),
 *            where φ_k are the coefficients of Φ_1144(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   divisors_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/divisors_of_uint64.h"

/* structure for a polynomial in varchar order asc; coefficients are seq_t */
struct Poly
{
    size_t deg;   /* degree */
    seq_t *c;     /* coefficients, c[i] for x^i, size deg+1 */
};

/* multiply two polynomials (a*b) */
static struct Poly poly_mult(const struct Poly *a, const struct Poly *b)
{
    size_t newdeg = a->deg + b->deg;
    seq_t *c = (seq_t *)calloc(newdeg + 1, sizeof(seq_t));
    for (size_t i = 0; i <= a->deg; ++i)
    {
        if (a->c[i] == 0) continue;
        for (size_t j = 0; j <= b->deg; ++j)
        {
            if (b->c[j] == 0) continue;
            c[i + j] += a->c[i] * b->c[j];
        }
    }
    struct Poly res = {newdeg, c};
    return res;
}

/* long division of monic divisor: dividend / divisor, remainder assumed zero */
static struct Poly poly_divide(const struct Poly *dividend, const struct Poly *divisor)
{
    size_t degA = dividend->deg;
    size_t degB = divisor->deg;
    size_t degQ = degA - degB;

    seq_t *r = (seq_t *)calloc(degA + 1, sizeof(seq_t));
    for (size_t i = 0; i <= degA; ++i) r[i] = dividend->c[i];

    seq_t *q = (seq_t *)calloc(degQ + 1, sizeof(seq_t));

    for (int64_t i = (int64_t)degA; i >= (int64_t)degB; --i)
    {
        seq_t coeff = r[i];
        if (coeff == 0) continue;
        size_t shift = (size_t)(i - (int64_t)degB);
        q[shift] = coeff;
        for (size_t j = 0; j <= degB; ++j)
        {
            r[shift + j] -= coeff * divisor->c[j];
        }
    }

    free(r);
    struct Poly res = {degQ, q};
    return res;
}

#define PERIOD 1144

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t seq[PERIOD];

    if (!initialized)
    {
        /* ---- step 1: build cyclotomic polynomials for all divisors of 1144 ---- */
        uint64_t divs[64];
        size_t dcnt;
        divisors_of_uint64(1144ULL, divs, &dcnt);

        struct Poly phi[64];
        size_t i;
        for (i = 0; i < dcnt; ++i)
        {
            uint64_t d = divs[i];
            /* dividend: x^d - 1 */
            seq_t *divC = (seq_t *)calloc(d + 1, sizeof(seq_t));
            divC[0] = (seq_t)-1;
            divC[d] = (seq_t)1;
            struct Poly dividend = {d, divC};

            /* product of φ_e for proper divisors e of d */
            seq_t *prodC = (seq_t *)calloc(1, sizeof(seq_t));
            prodC[0] = (seq_t)1;
            struct Poly prod = {0, prodC};

            for (size_t j = 0; j < i; ++j)
            {
                if (d % divs[j] == 0)
                {
                    struct Poly newProd = poly_mult(&prod, &phi[j]);
                    free(prod.c);
                    prod = newProd;
                }
            }

            /* φ_d = dividend / prod */
            struct Poly phiD = poly_divide(&dividend, &prod);

            /* clean up temporary objects */
            free(divC);
            free(prod.c);

            phi[i] = phiD;
        }

        /* ---- step 2: extract φ_1144 ---- */
        size_t id_1144 = 0;
        while (divs[id_1144] != 1144ULL) ++id_1144;
        struct Poly phi1144 = phi[id_1144];

        /* ---- step 3: compute periodic sequence a(0..PERIOD-1) ---- */
        seq[0] = (seq_t)1;
        for (size_t k = 1; k < PERIOD; ++k)
        {
            seq_t sum = (seq_t)0;
            size_t m = phi1144.deg < k ? phi1144.deg : k;
            for (size_t j = 1; j <= m; ++j)
                sum += phi1144.c[j] * seq[k - j];
            seq[k] = -sum;
        }

        /* free cyclotomic polynomials */
        for (i = 0; i < dcnt; ++i)
            free(phi[i].c);

        initialized = 1;
    }

    return seq[(size_t)(n % PERIOD)];
}
