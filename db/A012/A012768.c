/*
 * A012768  exp(cosh(x)*arctan(x))=1+x+1/2!*x^2+2/3!*x^3+5/4!*x^4+20/5!*x^5...
 * https://oeis.org/A012768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A012768:  a(n) = n! · [xⁿ] exp(cosh(x)*atan(x))
   (OEIS specification) */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Internal tables for factorials, h‑coefficients, and the sequence */
static seq_t *fact = NULL;   /* fact[i] = i!                         */
static size_t fact_cap = 0;   /* number of factorial entries           */

static seq_t *H = NULL;      /* H[k] = k! * coeff of x^k in h(x)      */
static size_t H_cap = 0;      /* table capacity                        */

static seq_t *seq_a = NULL;  /* seq_a[n] = a(n)                       */
static size_t a_cap = 0;      /* table capacity                        */

/* Ensure the factorial table reaches at least n.
   For int64_t builds we stop at 20 to avoid overflow. */
static void ensure_fact_cap(idx_t n)
{
    if ((idx_t)fact_cap > n) return;
    /* Cap at 20 for 64‑bit seq_t to stay in bounds. */
    idx_t limit = n;
    if (sizeof(seq_t) <= 8) {
        if (limit > 20) limit = 20;
    }
    size_t new_cap = (size_t)limit + 1;
    seq_t *new_fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
    if (!new_fact) abort();
    if (fact_cap == 0) {
        new_fact[0] = (seq_t)1;      /* 0! = 1 */
        fact_cap = 1;
    }
    for (size_t i = fact_cap; i < new_cap; ++i)
        new_fact[i] = new_fact[i-1] * (seq_t)i;
    fact = new_fact;
    fact_cap = new_cap;
}

/* Compute H[k] = k!·coeff of x^k in h(x)=cosh(x)*atan(x).  */
static void ensure_H_cap(idx_t n)
{
    if ((idx_t)H_cap > n) return;
    /* Need factorials up to n */
    ensure_fact_cap(n);
    size_t new_cap = (size_t)n + 1;
    seq_t *new_H = (seq_t*)realloc(H, new_cap * sizeof(seq_t));
    if (!new_H) abort();
    for (size_t k = H_cap; k < new_cap; ++k) {
        if ((k & 1) == 0) {
            new_H[k] = (seq_t)0;  /* even indices vanish */
            continue;
        }
        idx_t r = (idx_t)(k - 1) / 2;
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j <= r; ++j) {
            idx_t i = r - j;
            /* term = fact[k] / fact[2*i] / (2*j+1) * (-1)^j */
            seq_t term = fact[(size_t)k] / fact[(size_t)(2*i)];
            term /= (seq_t)(2*j + 1);
            if (j & 1) term = -term;
            sum += term;
        }
        new_H[k] = sum;
    }
    H = new_H;
    H_cap = new_cap;
}

/* Ensure the sequence table reaches at least n. */
static void ensure_a_cap(idx_t n)
{
    if ((idx_t)a_cap > n) return;
    ensure_H_cap(n);
    size_t new_cap = (size_t)n + 1;
    seq_t *new_a = (seq_t*)realloc(seq_a, new_cap * sizeof(seq_t));
    if (!new_a) abort();
    if (a_cap == 0) {
        new_a[0] = (seq_t)1;     /* a(0) = 1 */
        a_cap = 1;
    }
    for (size_t m = a_cap; m < new_cap; ++m) {
        seq_t sum = (seq_t)0;
        if (m == 0) {
            new_a[m] = (seq_t)1;
            continue;
        }
        /* binomial multiplier:  binom(m-1, k-1) for k=1..m */
        seq_t bin_mul = (seq_t)1;  /* binom(m-1,0) = 1, k=1 */
        for (idx_t k = 1; k <= m; ++k) {
            if (H[(size_t)k] != (seq_t)0) {
                seq_t term = H[(size_t)k] * bin_mul;
                term *= new_a[(size_t)(m - k)];
                sum += term;
            }
            /* update bin_mul for next k: binom(m-1, k) = binom(m-1, k-1) * (m-k)/k */
            if (k != m) {
                bin_mul = bin_mul * (seq_t)(m - k) / (seq_t)k;
            }
        }
        new_a[m] = sum;
    }
    seq_a = new_a;
    a_cap = new_cap;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    ensure_a_cap(n);
    return seq_a[(size_t)n];
}
