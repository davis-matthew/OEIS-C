/*
 * A012709  exp(arctanh(x)*exp(x))=1+x+3/2!*x^2+12/3!*x^3+57/4!*x^4+340/5!*x^5...
 * https://oeis.org/A012709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS sequence A012709 * a(n) = n!* [x^n] exp(arctanh(x)*exp(x))
 *            * We compute a(n) by recursion: * g[0] = 0 * g[n] = g[n-1] +
 *            Σ_{j=0}^{⌊(n-1)/2⌋} (n-1)! / (n-1-2j)! * a[0] = 1 * a[n] =
 *            Σ_{k=0}^{n-1} C(n-1,k) * g[k+1] * a[n-1-k] * * This gives the
 *            required values for all n ≥ 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Note: seq_t and idx_t are supplied by the build system.
 * They may be standard integer types or an arbitrary precision class
 * (in which case the same code is compiled as C++17).  All arithmetic
 * operators (+, -, *, /, %) are defined for seq_t. */

static seq_t *a_tbl = NULL;        /* array of a(n) values          */
static seq_t *g_tbl = NULL;        /* array of g(n) values          */
static idx_t  computed = 0;        /* largest n for which a_tbl[n] is valid */
static size_t allocated = 0;       /* current storage capacity       */

/* Expands the arrays to hold values up to n (inclusive) if necessary,
 * computing all intermediate terms. */
static void ensure_up_to(idx_t n)
{
    /* Grow the storage if required. */
    if (allocated < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *new_g = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (allocated > 0) {
            /* Copy existing values (operator= works for POD and C++ big‑int). */
            for (idx_t i = 0; i <= computed; ++i) {
                new_a[i] = a_tbl[i];
                new_g[i] = g_tbl[i];
            }
            free(a_tbl);
            free(g_tbl);
        } else {
            /* First time: set the base values a(0)=1 and g(0)=0. */
            new_a[0] = (seq_t)1;
            new_g[0] = (seq_t)0;
            computed = 0;
        }
        a_tbl = new_a;
        g_tbl = new_g;
        allocated = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* === compute g[i] === */
        seq_t sum = (seq_t)1;     /* term for j = 0 is always 1      */
        seq_t term = (seq_t)1;     /* term_j for j = 0                */
        idx_t half = (i - 1) / 2;  /* floor((i-1)/2)                  */
        for (idx_t j = 1; j <= half; ++j) {
            /* Multiply by (i-1-2(j-1)) * (i-2-2(j-1)) */
            term *= (seq_t)(i - 1 - 2 * (j - 1));
            term *= (seq_t)(i - 2 - 2 * (j - 1));
            sum += term;
        }
        g_tbl[i] = g_tbl[i - 1] + sum;

        /* === compute a[i] === */
        seq_t binom = (seq_t)1;    /* C(i-1,0)                         */
        seq_t acc   = (seq_t)0;
        for (idx_t k = 0; k <= i - 1; ++k) {
            seq_t tmp = binom;
            tmp *= g_tbl[k + 1];
            tmp *= a_tbl[i - 1 - k];
            acc += tmp;
            if (k < i - 1) {
                /* Update binomial coefficient to C(i-1,k+1)       */
                seq_t num = (seq_t)(i - 1 - k);
                seq_t den = (seq_t)(k + 1);
                binom *= num;
                binom /= den;   /* exact division */
            }
        }
        a_tbl[i] = acc;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_tbl[n];
}
