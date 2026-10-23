/*
 * A015351  Inverse of 1342nd cyclotomic polynomial.
 * https://oeis.org/A015351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 1342nd cyclotomic polynomial.
   a(n) is the coefficient of x^n in the formal power series
   1/Φ_{1342}(x)  modulo (x^{1342}-1).  The sequence is periodic
   with period 1342.  The algorithm first computes the coefficients
   of Φ_{1342}(x) by successive polynomial division and then
   generates the first 1342 terms from the recurrence
   a(0)=1,
   a(n) = -∑_{k=1}^{φ(1342)} c_k * a(n-k)   (n>0),
   where c_k are the coefficients of Φ_{1342}(x).  These
   1342 terms are cached for fast repeated evaluation.  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define PER 1342   /* period length */

/* exact polynomial division (numerator/denominator) – the division
   is exact and the denominator is monic.  The quotient is returned
   as a newly allocated array of length (ndeg - ddeg + 1) and
   *qdeg_out is set to the degree of the quotient. */
static seq_t *poly_div(const seq_t *num, size_t ndeg,
                       const seq_t *den, size_t ddeg,
                       size_t *qdeg_out)
{
    /* copy numerator into a writable buffer */
    seq_t *rem = (seq_t *)malloc((ndeg + 1) * sizeof(seq_t));
    memcpy(rem, num, (ndeg + 1) * sizeof(seq_t));

    size_t qdeg = ndeg - ddeg;
    seq_t *q = (seq_t *)calloc(qdeg + 1, sizeof(seq_t));

    /* long division, descending degree */
    for (long long i = (long long)ndeg; i >= (long long)ddeg; --i) {
        seq_t coef = rem[(size_t)i];
        size_t k = (size_t)(i - ddeg);
        q[k] = coef;

        /* subtract coef * den(x) shifted by k from the remainder */
        for (size_t j = 0; j <= ddeg; ++j)
            rem[k + j] -= coef * den[j];
    }
    free(rem);
    *qdeg_out = qdeg;
    return q;
}

/* Computes the periodic sequence a(n). */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t cache[PER];          /* first 1342 terms, periodic */
    static seq_t *phi[8] = {0};       /* coefficients of Φ_d for each divisor
                                         of 1342 */
    static size_t phi_deg[8] = {0};

    if (!initialized) {
        /* Divisors of 1342 in ascending order */
        const idx_t divs[8] = {1, 2, 11, 22, 61, 122, 671, 1342};

        /* Build Φ_d for each proper divisor first */
        for (size_t id = 0; id < 7; ++id) {
            idx_t d = divs[id];
            size_t ddeg = (size_t)d;
            seq_t *num = (seq_t *)calloc(ddeg + 1, sizeof(seq_t));
            num[0] = -1;         /* x^d - 1 */
            num[ddeg] = 1;
            size_t nd = ddeg;

            /* divide by all proper already–computed divisors of d */
            for (size_t ie = 0; ie < id; ++ie) {
                if (d % divs[ie] != 0) continue;  /* not a divisor */
                size_t qdeg;
                seq_t *q = poly_div(num, nd, phi[ie], phi_deg[ie], &qdeg);
                free(num);
                num = q;
                nd = qdeg;
            }
            phi[id] = num;
            phi_deg[id] = nd;   /* degree = φ(d) */
        }

        /* Now compute Φ_{1342}: divide by all proper divisors */
        idx_t d = divs[7];
        size_t ddeg = (size_t)d;
        seq_t *num = (seq_t *)calloc(ddeg + 1, sizeof(seq_t));
        num[0] = -1;
        num[ddeg] = 1;
        size_t nd = ddeg;

        for (size_t ie = 0; ie < 7; ++ie) {
            size_t qdeg;
            seq_t *q = poly_div(num, nd, phi[ie], phi_deg[ie], &qdeg);
            free(num);
            num = q;
            nd = qdeg;
        }
        phi[7] = num;              /* φ(1342) = 600 → degree 600 */
        phi_deg[7] = nd;

        /* Generate the first 1342 terms from the recurrence */
        cache[0] = 1;
        for (size_t i = 1; i < PER; ++i) {
            seq_t sum = 0;
            size_t maxk = (i < phi_deg[7]) ? i : phi_deg[7];
            for (size_t k = 1; k <= maxk; ++k) {
                sum += phi[7][k] * cache[i - k];
            }
            cache[i] = -sum;
        }

        initialized = 1;
    }

    /* Return the value, periodic with period 1342 */
    return cache[(size_t)(n % PER)];
}
