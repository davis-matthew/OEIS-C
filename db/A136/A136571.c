/*
 * A136571  Irregular triangle of coefficients of the minimal polynomial of
 *          2*cos(2*Pi/n) in decreasing powers.
 * https://oeis.org/A136571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 42, i16 n <= 305, i32 n <= 1313, i64 n <= 1623,
 *            i128 n <= 1623, big any n
 * Helpers:   gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A136571
 *
 *  This implementation computes the n‑th coefficient of the triangle of
 *  coefficients of the minimal polynomial of 2*cos(2*Pi/k).  The
 *  sequence is obtained by writing the polynomials row by row,
 *  flattening them into a single sequence.  Indexing follows the OEIS
 *  convention: a(1) is the first entry.
 *
 *  Algorithm:
 *      For a given row k (k >= 1) let
 *          D = φ(k)/2           (k = 1,2 special case)
 *          t_j = 2*cos(2π j/k)  (j coprime to k)
 *
 *      The power sums p_m = sum_j t_j^m are computed by
 *          p_m = (1/2) * Σ_{j=0}^{m} C(m,j) * c_k(m-2j)
 *      where C(m,j) are binomial coefficients and
 *          c_k(r) = Ramanujan sum = Σ_{gcd(s,k)=1} exp(2π i s r/k)
 *                  = Σ_{d|gcd(k,r)} d μ(k/d).
 *
 *      Using Newton's identities the elementary symmetric
 *      polynomials a_1 … a_D (coefficients of the monic
 *      polynomial) are obtained:
 *          a_k = -(p_k + Σ_{i=1}^{k-1} a_i p_{k-i}) / k.
 *
 *      The coefficient array for the row is
 *          coeff[0] = 1
 *          coeff[i] = a_i     (1 <= i <= D)
 *
 *      The sequence index is located by summing row lengths until
 *      the desired row is found, then the appropriate element is
 *      returned.
 *
 *  Complexity:  O(k^2) per row (k is the row number), trivial for k < ~300.
 *  Memory usage: O(k) per call.
 *
 *  Note:  All helper utilities are derived from the provided
 *  utility library.  Only standard C library functions are used.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd_idx_t.h"

/* ------------------------------------------------------------------ */
/* Local utility functions                                               */
/* ------------------------------------------------------------------ */

/* Euclidean GCD for idx_t */

/* φ(n) for small n (trial division) */
static idx_t phi_small(idx_t n)
{
    if (n == 0) return 0;
    idx_t result = n;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result -= result / p;
        }
    }
    if (t > 1) result -= result / t;
    return result;
}

/* Möbius function μ(n) for small n */
static int mu_small(idx_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            if (exp > 1) return 0;   /* squared prime factor */
            cnt++;
        }
    }
    if (t > 1) cnt++;                /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;
}

/* Ramanujan sum c_k(r) */
static seq_t c_n_r(idx_t k, idx_t r)
{
    if (r == 0) return (seq_t)phi_small(k);

    idx_t g = gcd_idx_t(k, r);
    seq_t sum = 0;

    /* iterate over divisors of g */
    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            idx_t d1 = d;
            sum += (seq_t)d1 * (seq_t)mu_small(k / d1);
            if (d1 * d1 != g) {
                idx_t d2 = g / d1;
                sum += (seq_t)d2 * (seq_t)mu_small(k / d2);
            }
        }
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Compute the coefficient array for row k                          */
/* ------------------------------------------------------------------ */
static void compute_row_coeffs(idx_t k, seq_t **coeff_out, size_t *len_out)
{
    if (k == 1) {
        *len_out = 2;
        *coeff_out = (seq_t *)malloc(2 * sizeof(seq_t));
        (*coeff_out)[0] = (seq_t)1;
        (*coeff_out)[1] = (seq_t)-2;
        return;
    }
    if (k == 2) {
        *len_out = 2;
        *coeff_out = (seq_t *)malloc(2 * sizeof(seq_t));
        (*coeff_out)[0] = (seq_t)1;
        (*coeff_out)[1] = (seq_t)2;
        return;
    }

    idx_t phi_k = phi_small(k);
    idx_t D = phi_k / 2;                  /* degree of univariate polynomial */
    size_t len = D + 1;                   /* number of coefficients */

    /* power sums p[1..D] */
    seq_t *p = (seq_t *)calloc(D + 1, sizeof(seq_t));

    for (idx_t m = 1; m <= D; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;                  /* C(m,0) */
        for (idx_t j = 0; j <= m; ++j) {
            idx_t r = (idx_t)(m > 2 * j ? m - 2 * j : 2 * j - m);  /* |m-2j| */
            seq_t c = c_n_r(k, r);
            sum += binom * c;
            if (j < m) {
                binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);    /* next binomial */
            }
        }
        p[m] = sum / (seq_t)2;   /* elements appear twice except n=2 (handled above) */
    }

    /* Newton identities to get polynomial coefficients a[0..D] */
    seq_t *a = (seq_t *)malloc(len * sizeof(seq_t));
    a[0] = (seq_t)1;   /* leading coefficient */

    for (idx_t kidx = 1; kidx <= D; ++kidx) {
        seq_t temp = p[kidx];
        for (idx_t i = 1; i < kidx; ++i) {
            temp += a[i] * p[kidx - i];
        }
        a[kidx] = -temp / (seq_t)kidx;
    }

    free(p);

    /* output array */
    *coeff_out = a;
    *len_out = len;
}

 /* ------------------------------------------------------------------ */
 /* SEQUENCE entry function                                           */
 /* ------------------------------------------------------------------ */
seq_t a(idx_t index)
{
    if (index < 1) return (seq_t)0;   /* undefined, but safe */

    /* locate row */
    idx_t row = 1;
    idx_t cumulative = 0;
    while (1) {
        idx_t row_len;
        if (row == 1 || row == 2) row_len = 2;
        else                       row_len = phi_small(row) / 2 + 1;

        if (cumulative + row_len >= index) break;
        cumulative += row_len;
        ++row;
    }

    idx_t offset = index - cumulative - 1;   /* 0‑based in the row */

    seq_t *coeff;
    size_t coeff_len;
    compute_row_coeffs(row, &coeff, &coeff_len);

    seq_t result = coeff[offset];
    free(coeff);
    return result;
}
