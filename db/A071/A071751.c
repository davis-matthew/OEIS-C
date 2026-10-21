/*
 * A071751  Expansion of (1+x^4*C^3)*C^2, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff of x^n in (1 + x^4*C^3)*C^2 * = coeff(C^2, n) +
 *            coeff(C^5, n-4) (with coeff(C^5, m)=0 for m<0) * where C(x) =
 *            (1-√(1-4x))/(2x) is the Catalan generating function
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  ----  Dynamic arrays  ----  */
static seq_t *catalan = NULL;   /* Catalan numbers C_n   */
static seq_t *c2      = NULL;   /* coefficients of C^2   */
static seq_t *c3      = NULL;   /* coefficients of C^3   */
static seq_t *c5      = NULL;   /* coefficients of C^5   */

static size_t capC  = 0;        /* current allocation sizes */
static size_t capC2 = 0;
static size_t capC3 = 0;
static size_t capC5 = 0;

static idx_t  max_n = 0;        /* largest n that has been computed */

/*  ---- Helper to grow an array  ----  */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : *cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }       /* out of memory – abort */
        *arr = tmp;
        *cap = new_cap;
    }
}

/*  ---- Extend all arrays up to n (inclusive)  ----  */
static void extend_to(idx_t n)
{
    if (n < max_n) return;          /* already done */

    ensure_cap(&catalan, &capC,  (size_t)(n + 1));
    ensure_cap(&c2,      &capC2, (size_t)(n + 1));
    ensure_cap(&c3,      &capC3, (size_t)(n + 1));
    ensure_cap(&c5,      &capC5, (size_t)(n + 1));

    for (idx_t i = max_n; i <= n; ++i) {
        /* Catalan numbers: C_0 = 1,  C_n = C_{n-1} * 2*(2*n-1)/(n+1) */
        if (i == 0) {
            catalan[0] = 1;
        } else {
            seq_t num   = (seq_t)(2 * (2 * i - 1));
            seq_t denom = (seq_t)(i + 1);
            catalan[i] = (catalan[i - 1] * num) / denom;
        }

        /* Coefficients of C^2  (convolution of Catalan with itself) */
        seq_t sum2 = 0;
        for (idx_t j = 0; j <= i; ++j) {
            sum2 += catalan[j] * catalan[i - j];
        }
        c2[i] = sum2;

        /* Coefficients of C^3  (= C^2 * C) */
        seq_t sum3 = 0;
        for (idx_t j = 0; j <= i; ++j) {
            sum3 += c2[j] * catalan[i - j];
        }
        c3[i] = sum3;

        /* Coefficients of C^5  (= C^2 * C^3) */
        seq_t sum5 = 0;
        for (idx_t j = 0; j <= i; ++j) {
            sum5 += c2[j] * c3[i - j];
        }
        c5[i] = sum5;
    }

    max_n = n + 1;
}

/*  ---- Public function required by the judge  ----  */
seq_t a(idx_t n)
{
    extend_to(n);
    seq_t res = c2[n];
    if (n >= 4) res += c5[n - 4];
    return res;
}
