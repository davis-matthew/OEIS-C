/*
 * A117250  Triangle T, read by rows, where matrix power T^2 has powers of 2
 *          in the secondary diagonal: [T^2](n+1,n) = 2^(n+1), with all 1's in
 *          the main diagonal and zeros elsewhere.
 * https://oeis.org/A117250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 20, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117250: a(n) is the nth element of the lower‑triangular triangle
   T with the property that the secondary diagonal of T^2 consists of
   powers of two.  For n≥0, a(n)=T(r,c) where r,c satisfy
       n = r(r+1)/2 + c , 0≤c≤r .
   The triangle entry is
       T(r,c)=∏_{j=0}^{d-1}(1-2j)/(d)! · 2^{r(r-1)/2 – c(c-1)/2}
   with d=r‑c.  The division is handled exactly using the fact that the
   odd part of d! divides the odd product.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

#define MAX_PRECOMP 2000

static seq_t fact_arr[MAX_PRECOMP];
static seq_t odd_prod_arr[MAX_PRECOMP];
static idx_t precomp_cap = 0;

/* Compute factorial and odd‑product up to d. */
static void ensure_precomp(idx_t d)
{
    if (precomp_cap == 0) {
        fact_arr[0] = (seq_t)1;
        odd_prod_arr[0] = (seq_t)1;
        precomp_cap = 1;
    }
    if (d < precomp_cap) return;
    if (d >= MAX_PRECOMP) abort();
    for (idx_t i = precomp_cap; i <= d; ++i) {
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;
        seq_t factor = (seq_t)1 - (seq_t)2 * (seq_t)(i-1);
        odd_prod_arr[i] = odd_prod_arr[i-1] * factor;
    }
    precomp_cap = d + 1;
}

/* Return the 2‑adic exponent of d! (sum floor(d/2^k)). */
static idx_t v2_factorial(idx_t d)
{
    idx_t v2 = 0;
    for (idx_t k = 2; k <= d; k <<= 1)
        v2 += d / k;
    return v2;
}

/* Convert linear index n to row r and column c. */
static void index_to_row_col(idx_t n, idx_t *r, idx_t *c)
{
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;
    *r = row;
    *c = n - row * (row + 1) / 2;
}

/* Return the nth term of A117250 (0‑based). */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_row_col(n, &r, &c);
    idx_t d = r - c;                     /* row‑col difference */
    ensure_precomp(d);

    /* odd part of numerator and denominator */
    seq_t oddProd = odd_prod_arr[d];
    seq_t fac      = fact_arr[d];
    idx_t v2Fact   = v2_factorial(d);
    seq_t oddDenom = fac / pow2(v2Fact); /* remove all factors 2 */
    seq_t part     = oddProd / oddDenom; /* exact integer */

    /* exponent of 2 in the final product */
    seq_t expVal = (seq_t)r * ((seq_t)r - 1) / 2
                 - (seq_t)c * ((seq_t)c - 1) / 2;
    idx_t expo = (idx_t)(expVal - (seq_t)v2Fact); /* must be >=0 */
    seq_t pow2_val = pow2(expo);

    return part * pow2_val;
}
