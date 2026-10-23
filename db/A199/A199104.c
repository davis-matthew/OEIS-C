/*
 * A199104  G.f. satisfies: A(x) = exp( Sum_{n>=1} A(x^n)/(1-x)^n * x^n/n ).
 * https://oeis.org/A199104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199104: a(n) defined by the functional equation
     A(x) = exp( Σ_{m≥1} A(x^m)/(1−x)^m * x^m/m )
   The implementation computes coefficients iteratively.
   It keeps auxiliary arrays for the sequence a(n) and
   for the integer values s(n) = n·l(n) where
   l(n) are the coefficients of log A(x).
   See the recurrence:
        n·a(n) = Σ_{k=1}^{n} s(k) · a(n−k)   ,  a(0)=1
   where
        s(k) = k·l(k) = Σ_{d=1}^{k} (k/d) ·
                 Σ_{m≥0, d(m+1)≤k} a(m) · binom(d + k−d(m+1)−1, k−d(m+1))
   Binomial coefficients are computed by the helper binom().
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

static seq_t *A = NULL;   /* coefficients a(n) */
static seq_t *S = NULL;   /* s(n) = n·l(n) */
static idx_t cap = 0;     /* allocated size */
static idx_t term_cnt = 0;/* last computed index */

/* Ensure arrays large enough and compute terms up to n. */
static void ensure(idx_t n)
{
    if (n <= term_cnt) return;

    /* grow storage when necessary */
    idx_t new_cap = cap ? cap : 1;
    while (new_cap <= n) new_cap <<= 1;
    if (A == NULL) {
        A = (seq_t *)malloc(new_cap * sizeof(seq_t));
        S = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
    }
    if (!A || !S) abort();          /* allocation failure */
    cap = new_cap;

    /* initialise a(0) if not done yet */
    if (term_cnt == 0) {
        A[0] = (seq_t)1;
        term_cnt = 0;
    }

    /* compute terms from term_cnt+1 up to n */
    for (idx_t t = term_cnt + 1; t <= n; ++t) {
        /* -------- compute s(t) -------- */
        seq_t st = (seq_t)0;
        for (idx_t d = 1; d <= t; ++d) {
            seq_t inner = (seq_t)0;
            idx_t mmax = (t - d) / d;          /* floor((t-d)/d) */
            for (idx_t m = 0; m <= mmax; ++m) {
                idx_t tprime = t - d * (m + 1);   /* ≥0 */
                seq_t bin = binom(d + tprime - 1, tprime);
                inner += A[m] * bin;
            }
            seq_t tmp = inner * (seq_t)t;  /* multiply by t */
            tmp /= (seq_t)d;               /* exact division by d */
            st += tmp;
        }
        S[t] = st;

        /* -------- compute a(t) -------- */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= t; ++k) {
            sum += S[k] * A[t - k];
        }
        A[t] = sum / (seq_t)t;           /* integer division, exact */
    }

    term_cnt = n;
}

/* Public routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure(n);
    return A[n];
}
