/*
 * A013464  arctan(arctan(x)-arctanh(x))=-4/3!*x^3-1440/7!*x^7+35840/9!*x^9...
 * https://oeis.org/A013464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A013464
    a(n) = n!  * [x^n] arctan( arctan(x) – artanh(x) )
    The sequence is computed by composing power series in their
    exponential–generating‑function representation.  All arithmetic is
    performed on elements of type seq_t, using only integer operators.
*/
#include <stddef.h>
#include <stdlib.h>

/* binomial coefficient using the pre‑computed factorials */
static inline seq_t binom(idx_t n, idx_t k, const seq_t *fact)
{
    return fact[n] / (fact[k] * fact[n - k]);
}

seq_t a(idx_t n)
{
    /* required exponent */
    idx_t M = 2 * n + 1;

    /* ---------- factorials 0! .. M! ---------- */
    seq_t *fact = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
    if (!fact) return 0;
    fact[0] = 1;
    for (idx_t i = 1; i <= M; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* ---------- g(x) = arctan(x) – artanh(x) ---------- */
    seq_t *g = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
    idx_t pos_count = (M >= 3) ? ((M - 3) / 4 + 1) : 0;
    idx_t *pos = (idx_t *)((pos_count > 0) ? malloc(pos_count * sizeof(idx_t)) : NULL);
    idx_t pc = 0;
    for (idx_t i = 3; i <= M; i += 4) {
        pos[pc++] = i;
        g[i] = - (seq_t)2 * fact[i - 1];   /* a_g[i] = -2*(i-1)! */
    }

    /* ---------- s(x) = g(x)^2 ---------- */
    seq_t *s = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
    if (pos_count > 0) {
        for (idx_t ii = 0; ii < pos_count; ++ii) {
            idx_t i = pos[ii];
            for (idx_t jj = 0; jj < pos_count; ++jj) {
                idx_t j = pos[jj];
                idx_t k = i + j;
                if (k > M) continue;
                seq_t term = (seq_t)4 * fact[k] / ((seq_t)i * (seq_t)j);
                s[k] += term;
            }
        }
    }

    /* ---------- b(x) = 1/(1 + s(x)) ---------- */
    seq_t *b = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
    b[0] = 1;
    for (idx_t m = 1; m <= M; ++m) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= m; ++i) {
            if (s[i] == 0) continue;
            seq_t term = binom(m, i, fact) * s[i] * b[m - i];
            sum += term;
        }
        b[m] = -sum;
    }

    /* ---------- h(x) = g'(x) * b(x) ---------- */
    seq_t *h = (seq_t *)calloc((size_t)M, sizeof(seq_t)); /* indices 0 .. M-1 */
    for (idx_t m = 0; m < M; ++m) {
        seq_t sum = 0;
        for (idx_t ii = 0; ii < pos_count; ++ii) {
            idx_t j = pos[ii];            /* g index with non‑zero value           */
            if (j > M) continue;          /* safety: j <= M by construction       */
            idx_t i = j - 1;              /* derivative index                     */
            if (i > m) continue;
            seq_t term = binom(m, i, fact) * g[j] * b[m - i];
            sum += term;
        }
        h[m] = sum;
    }

    /* ---------- answer a(n) = f_scaled[2*n+1] = h[2*n] ---------- */
    seq_t result = h[2 * n];

    /* ---------- clean up ---------- */
    free(fact);
    free(g);
    free(pos);
    free(s);
    free(b);
    free(h);
    return result;
}
