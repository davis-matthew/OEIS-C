/*
 * A069251  Number of basis partitions of n+64 with Durfee square size 8.
 * https://oeis.org/A069251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 164, u64 n <= 3936,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069251: Number of basis partitions of n+64 with Durfee square size 8.
 *  Recurrence derived from Q(x)·A(x)=P(x) where
 *    Q(x) = (1‑x)^8 (x^2+x+1)^2 (x^4+x^3+x^2+x+1) (x^6+…+1)
 *    P(x) = (x^4‑x^3+x^2‑x+1)(x^4‑x^2+1)(x^6‑x^5+…+1)(x^8+1)
 *  Let q_i (i=0..22) and p_i (i=0..22) be the coefficients of Q and P.
 *  Then for every n≥0  (q_0=1)
 *
 *      a(n) = p_n         –  ∑_{k=1}^{min(n,22)} q_k · a(n-k)
 *
 *  (For n≥23 the term p_n is 0, i.e. the right–hand side contains only the sum.)
 *
 *  The implementation pre‑computes the 23‑term coefficient vectors
 *  and generates terms on demand using the recurrence.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Coefficients of Q(x) and P(x).  Size 23 = degree 22 + 1. */
static int64_t Q_coeff[23];
static int64_t P_coeff[23];
static bool coeffs_init = false;

/* Multiply two integer polynomials: res = a * b.
 *  len_a, len_b are the numbers of coefficients in a and b. */
static void poly_mul(int64_t *res, const int64_t *a, size_t len_a,
                     const int64_t *b, size_t len_b)
{
    size_t len_res = len_a + len_b - 1;
    for (size_t i = 0; i < len_res; ++i) res[i] = 0;
    for (size_t i = 0; i < len_a; ++i)
        for (size_t j = 0; j < len_b; ++j)
            res[i + j] += a[i] * b[j];
}

/* Initialise the Q_coeff and P_coeff tables. */
static void init_coeffs(void)
{
    if (coeffs_init) return;
    coeffs_init = true;

    /* ----- Numerator P(x) ----- */
    const int64_t A1[5] = {  1, -1,  1, -1,  1 };      /* x^4 – x^3 + x^2 – x + 1 */
    const int64_t A2[5] = {  1,  0, -1,  0,  1 };      /* x^4 – x^2 + 1      */
    const int64_t A3[7] = {  1, -1,  1, -1,  1, -1,  1 }; /* x^6 – x^5 + … – x + 1 */
    const int64_t A4[9] = {  1,  0,  0,  0,  0,  0,  0,  0,  1 }; /* x^8 + 1 */

    int64_t tmp1[9];
    int64_t tmp2[15];
    int64_t tmp3[23];
    poly_mul(tmp1, A1, 5, A2, 5);          /* degree 8 */
    poly_mul(tmp2, tmp1, 9, A3, 7);        /* degree 14 */
    poly_mul(tmp3, tmp2, 15, A4, 9);       /* degree 22 */
    for (int i = 0; i < 23; ++i) P_coeff[i] = tmp3[i];

    /* ----- Denominator Q(x) ----- */
    const int64_t B1[9] = {  1, -8,  28, -56,  70, -56,  28, -8,  1 }; /* (1‑x)^8 */
    const int64_t B2[5] = {  1,  2,   3,   2,  1 };                     /* (x^2+x+1)^2 */
    const int64_t B3[5] = {  1,  1,   1,   1,  1 };                     /* (x^4+…+1) */
    const int64_t B4[7] = {  1,  1,   1,   1,  1,  1,  1 };             /* (x^6+…+1) */

    int64_t tmp_q1[13];
    int64_t tmp_q2[17];
    int64_t tmp_q3[23];
    poly_mul(tmp_q1, B1, 9, B2, 5);          /* degree 12 */
    poly_mul(tmp_q2, tmp_q1, 13, B3, 5);     /* degree 16 */
    poly_mul(tmp_q3, tmp_q2, 17, B4, 7);     /* degree 22 */
    for (int i = 0; i < 23; ++i) Q_coeff[i] = tmp_q3[i];
}

/* Buffer of already computed a(n) values. */
static seq_t *seq_buf = NULL;
static idx_t   buf_len = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_coeffs();

    /* Grow the buffer and compute missing terms. */
    if (n >= buf_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_buf, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure; unlikely */
        seq_buf = tmp;

        for (idx_t k = buf_len; k <= n; ++k) {
            /* Start with the numerator term p_k (zero if k≥23). */
            seq_t val = 0;
            if (k < 23) val = (seq_t)P_coeff[(size_t)k];

            /* Subtract the contribution from the denominator terms. */
            size_t limit = (k < 22) ? (size_t)k : 22;
            for (size_t j = 1; j <= limit; ++j) {
                int64_t coeff = Q_coeff[(size_t)j];
                seq_t prod = seq_buf[(size_t)(k - j)] *
                             (seq_t)(coeff >= 0 ? coeff : -coeff);
                if (coeff >= 0)
                    /* q_j positive: subtract q_j * a(k-j) */
                    val -= prod;
                else
                    /* q_j negative: subtract q_j * a(k-j) = +abs(q_j)*a(k-j) */
                    val += prod;
            }

            seq_buf[(size_t)k] = val;
        }
        buf_len = (size_t)n + 1;
    }
    return seq_buf[(size_t)n];
}
