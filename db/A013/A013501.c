/*
 * A013501  Expansion of e.g.f.:
 *          exp(sec(x)-exp(x))=1-x+1/2!*x^2-2/3!*x^3+9/4!*x^4-32/5!*x^5...
 * https://oeis.org/A013501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A013501 – Expansion of exp(sec(x) – exp(x)) as an e.g.f.
    a(n) is defined by the exponential generating function
        A(x) = exp(sec(x) – exp(x))
    The coefficients a(n) satisfy
        a(0) = 1,
        a(n) = Σ_{k=1}^{n} binom(n-1, k-1) * b(k) * a(n-k)   (n ≥ 1)
    where b(k) is the k‑th coefficient of the e.g.f. sec(x) – exp(x), i.e.
        B(x) = Σ_{k≥0} b(k) x^k / k!   with
        b(k) = s(k) – 1,
    and s(k) is the k‑th coefficient of sec(x).
    s(k) is obtained by recursive inversion of the e.g.f. of cos(x):
        s(0) = 1,
        s(m) = – Σ_{j=0}^{m-1} binom(m, j) * s(j) * c(m−j),   where
            c(t) = 0 if t is odd,
            c(t) = (−1)^{t/2} if t is even.
    All computations are done with the type seq_t; no floating point is used. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *s = NULL;
    static seq_t *b = NULL;
    static seq_t *a_seq = NULL;
    static idx_t max_n = (idx_t)-1;

    /* initialise on first call */
    if (max_n == (idx_t)-1) {
        max_n = 0;
        s = (seq_t *)malloc(sizeof(seq_t));
        b = (seq_t *)malloc(sizeof(seq_t));
        a_seq = (seq_t *)malloc(sizeof(seq_t));
        s[0] = (seq_t)1;              /* sec(0) = 1 */
        b[0] = (seq_t)0;              /* sec(0)-exp(0) = 0 */
        a_seq[0] = (seq_t)1;          /* a(0) = 1 */
        if (!s || !b || !a_seq) exit(1);
    }

    if (n == 0)
        return a_seq[0];

    if (n > max_n) {
        size_t new_len = (size_t)(n + 1);
        s = (seq_t *)realloc(s, sizeof(seq_t) * new_len);
        b = (seq_t *)realloc(b, sizeof(seq_t) * new_len);
        a_seq = (seq_t *)realloc(a_seq, sizeof(seq_t) * new_len);
        if (!s || !b || !a_seq) exit(1);

        /* compute s(k) for new indices */
        for (idx_t m = max_n + 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1;   /* binom(m,0) */
            for (idx_t k = 0; k < m; ++k) {
                /* coefficient of cos(x) : c(m-k) */
                if (((m - k) & 1) == 0) {        /* even index */
                    idx_t half = (m - k) >> 1;   /* (m-k)/2 */
                    seq_t term = bin * s[k];
                    if (half & 1)                /* (-1)^(half) */
                        term = -term;
                    sum += term;
                }
                /* update binomial coefficient for next k */
                bin = (bin * ((seq_t)(m - k))) / ((seq_t)(k + 1));
            }
            s[m] = -sum;   /* s(m) = -Σ ... */
        }

        /* compute b(k) = s(k) – 1 for new indices */
        for (idx_t m = max_n + 1; m <= n; ++m)
            b[m] = s[m] - (seq_t)1;

        /* compute a(n) via recurrence */
        for (idx_t m = max_n + 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1;   /* binom(m-1,0) */
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = bin * b[k] * a_seq[m - k];
                sum += term;
                /* next binomial coefficient: binom(m-1,k) */
                bin = (bin * ((seq_t)(m - k))) / ((seq_t)k);
            }
            a_seq[m] = sum;
        }

        max_n = n;
    }

    return a_seq[n];
}
