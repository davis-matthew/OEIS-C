/*
 * A073079  Generating function satisfies A(x) = exp(A(x)x + 3A(x^2)x^2/2 +
 *          A(x^3)x^3/3 + 3A(x^4)x^4/4 +...).
 * https://oeis.org/A073079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A(x) = exp( Σ_{n≥1} (2+(-1)^n) A(xⁿ) xⁿ / n ) * Recurrence (for
 *            n≥1): * a(0) = 1 * Let k*b_k = Σ_{d|k} (2+(-1)^d) a(k/d-1) *
 *            (k/d) * then * a(n) = (1/n) * Σ_{k=1..n} (k*b_k) * a(n-k) (int
 *            division)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* global arrays storing the sequence and the computed k*b_k values */
static seq_t *terms = NULL;    /* a[0]..a[n]  */
static seq_t *kbarr = NULL;   /* kbarr[0] unused, kbarr[1]..kbarr[n] */

static size_t terms_cap = 0;   /* allocated capacity of the arrays   */
static idx_t terms_len  = 0;   /* number of terms currently computed (terms_len = highest index + 1) */

/* ensure that terms[0..n] and kbarr[1..n] are computed */
static void ensure_terms(idx_t n)
{
    if (terms_len > n)
        return;                   /* already computed */

    /* Grow arrays to accommodate up to n */
    size_t new_cap = terms_cap ? terms_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    if (new_cap != terms_cap) {
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        kbarr = (seq_t *)realloc(kbarr, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }

    /* Compute missing terms, starting from current length */
    for (idx_t i = terms_len; i <= n; ++i) {
        if (i == 0) {                 /* base case a(0) = 1 */
            terms[0] = 1;
            /* kbarr[0] is unused */
            continue;
        }

        /* ----- compute k*b_k for k = i ----- */
        seq_t sum_kb = 0;
        /* iterate over divisors d of i */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                /* divisor d */
                idx_t coeff_d  = (d & 1) ? 1 : 3;      /* 2 + (-1)^d: 1 for odd, 3 for even */
                idx_t f_d      = i / d;                /* (k/d) factor */
                seq_t term_d   = terms[f_d - 1];         /* a(k/d-1) */
                sum_kb += (seq_t)coeff_d * term_d * (seq_t)f_d;

                /* if d and i/d are distinct, handle the paired divisor */
                idx_t d2 = i / d;
                if (d2 != d) {
                    idx_t coeff_d2 = (d2 & 1) ? 1 : 3;
                    idx_t f_d2     = i / d2;            /* = d */
                    seq_t term_d2  = terms[f_d2 - 1];
                    sum_kb += (seq_t)coeff_d2 * term_d2 * (seq_t)f_d2;
                }
            }
        }
        kbarr[i] = sum_kb;

        /* ----- compute a(i) using the recurrence ----- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += kbarr[k] * terms[i - k];
        }
        terms[i] = sum / (seq_t)i;
    }

    terms_len = n + 1;          /* now we have terms[0..n] */
}

/* external interface required by the runner */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[n];
}
