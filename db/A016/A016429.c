/*
 * A016429  1785th cyclotomic polynomial.
 * https://oeis.org/A016429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   The coefficients a(n) of the 1785‑th cyclotomic polynomial
 *            Φ1785(x). a(0) is the constant term, a(1) the coefficient of x,
 *            etc. The coefficients are obtained by dividing (x^1785 – 1) by
 *            the product of (x^d – 1) over all divisors d≠1785 with
 *            μ(1785/d)=−1, and keeping the remaining factors with μ=+...
 * Range:     i8 n <= 768, i16 n <= 768, i32 n <= 768, i64 n <= 768,
 *            i128 n <= 768, big any n
 * Validated: full, pass, 769 terms through n = 768 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Multiply a polynomial poly[0..len-1] by (x^d – 1).
   Result has length len+d and is returned in newly allocated memory.  */
static seq_t *multiply_by_factor(const seq_t *poly, size_t len, uint64_t d)
{
    size_t new_len = len + (size_t)d;
    seq_t *res = mem_new(seq_t, new_len);
    for (size_t i = 0; i < len; ++i) {
        seq_t val = poly[i];
        res[i]       -= val;                 /* minus 1 * x^0 */
        res[i + d]   += val;                 /* plus 1 * x^d */
    }
    return res;
}

/* Return Möbius μ(m) for 1≤m≤1785.  */
static int mobius_small(uint64_t m)
{
    int cnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            m /= p;
            ++cnt;
            if (m % p == 0) return 0;          /* squared prime factor */
            while (m % p == 0) m /= p;         /* remove all powers of p */
        }
    }
    if (m > 1) ++cnt;                        /* last prime factor */
    return (cnt & 1) ? -1 : 1;
}

/* Exact polynomial division: num/den = quot   (den divides num).  */
static void poly_divide(const seq_t *num, size_t num_len,
                        const seq_t *den, size_t den_len,
                        seq_t *quot)
{
    size_t deg_num = num_len - 1;
    size_t deg_den = den_len - 1;
    seq_t *rem = mem_new(seq_t, num_len);
    for (size_t i = 0; i < num_len; ++i) rem[i] = num[i];

    for (int i = (int)deg_num; i >= (int)deg_den; --i) {
        seq_t coeff = rem[i];
        if (coeff == 0) continue;
        size_t shift = (size_t)(i - (int)deg_den);
        quot[shift] = coeff;                  /* coefficient of x^shift */
        for (size_t j = 0; j < den_len; ++j) {
            rem[shift + j] -= coeff * den[j];
        }
    }
    mem_free(rem);
}

/* Cached coefficients of Φ1785(x).  */
static seq_t *coeffs = NULL;
static size_t coeff_len = 0;

/* Return the coefficient a(n).  */
seq_t a(idx_t n)
{
    if (!coeffs) {                           /* first call – build table */
        const uint64_t N = 1785;
        /* all divisors of 1785, sorted ascending */
        const uint64_t divisors[] = {1,3,5,7,15,17,21,35,51,85,
                                     105,119,255,357,595,1785};
        const size_t divcnt = sizeof(divisors) / sizeof(divisors[0]);

        uint64_t pos_d[16] = {0}, neg_d[16] = {0};
        size_t poscnt = 0, negcnt = 0;
        for (size_t i = 0; i < divcnt; ++i) {
            uint64_t d = divisors[i];
            int mu = mobius_small(N / d);
            if (mu == 1)       pos_d[poscnt++] = d;
            else if (mu == -1) neg_d[negcnt++] = d;
        }

        /* Build numerator polynomial = Π (x^d – 1) for μ = +1 */
        seq_t *num = mem_new(seq_t, 1);   /* polynomial 1 */
        num[0] = 1;
        size_t num_len = 1;
        for (size_t i = 0; i < poscnt; ++i) {
            seq_t *tmp = multiply_by_factor(num, num_len, pos_d[i]);
            mem_free(num);
            num      = tmp;
            num_len += (size_t)pos_d[i];
        }

        /* Build denominator polynomial = Π (x^d – 1) for μ = –1 */
        seq_t *den = mem_new(seq_t, 1);   /* polynomial 1 */
        den[0] = 1;
        size_t den_len = 1;
        for (size_t i = 0; i < negcnt; ++i) {
            seq_t *tmp = multiply_by_factor(den, den_len, neg_d[i]);
            mem_free(den);
            den      = tmp;
            den_len += (size_t)neg_d[i];
        }

        /* Φ1785(x) = num / den           */
        size_t deg_num = num_len - 1;
        size_t deg_den = den_len - 1;
        coeff_len = deg_num - deg_den + 1;        /* φ(1785) = 769 */
        coeffs = mem_new(seq_t, coeff_len);
        poly_divide(num, num_len, den, den_len, coeffs);

        mem_free(num);
        mem_free(den);
    }

    if ((idx_t)n >= coeff_len) return 0;
    return coeffs[n];
}
