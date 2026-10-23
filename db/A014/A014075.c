/*
 * A014075  Inverse of 66th cyclotomic polynomial.
 * https://oeis.org/A014075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient sequence of the formal power series
 *            1/Φ66(x). Here Φ66(x) = ∏_{d|66} (x^{66/d} - 1)^{μ(d)} is the
 *            66‑th cyclotomic polynomial. The coefficients satisfy a(0) = 1
 *            and for n > 0: a(n) = -∑_{k=1}^{min(n,20)} φ_k · a(n−k), where
 *            φ_k are the coefficents of Φ66(x). The sequence i...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* --------- Helper for polynomial manipulation --------- */
static void multiply_by_xd_minus1(const seq_t *c, size_t len,
                                  size_t d, seq_t *out, size_t *out_len)
{
    size_t new_len = len + d;
    for (size_t i = 0; i < new_len; ++i)
        out[i] = 0;
    for (size_t i = 0; i < len; ++i) {
        out[i] += c[i];
        out[i + d] -= c[i];
    }
    *out_len = new_len;
}

/* --------- Static state (lazy initialization) --------- */
static seq_t phi_coeff[21];     /* coefficients of Φ66(x)   */
static seq_t cache[66];         /* first 66 values of a(n)  */
static bool initialized = false;

/* --------- Build the Φ66(x) coefficients and 1-period cache --------- */
static void init_table(void)
{
    /* 66 = 2 · 3 · 11  -> divisors */
    static const size_t pos_divs[] = {1, 6, 22, 33};   /* μ(d)=+1 */
    static const size_t neg_divs[] = {2, 3, 11, 66};   /* μ(d)=-1 */

    seq_t P[200] = {0}, Q[200] = {0}, tmp[200] = {0};
    size_t lenP = 1, lenQ = 1;
    P[0] = 1;   /* start with polynomial 1 */
    Q[0] = 1;

    /* Build numerator: product of (x^{66/d} - 1) for μ(d)=+1 */
    for (size_t idx = 0; idx < sizeof(pos_divs)/sizeof(pos_divs[0]); ++idx) {
        size_t d = 66 / pos_divs[idx];
        multiply_by_xd_minus1(P, lenP, d, tmp, &lenP);
        for (size_t i = 0; i < lenP; ++i) P[i] = tmp[i];
    }

    /* Build denominator: product of (x^{66/d} - 1) for μ(d)=-1 */
    for (size_t idx = 0; idx < sizeof(neg_divs)/sizeof(neg_divs[0]); ++idx) {
        size_t d = 66 / neg_divs[idx];
        multiply_by_xd_minus1(Q, lenQ, d, tmp, &lenQ);
        for (size_t i = 0; i < lenQ; ++i) Q[i] = tmp[i];
    }

    /* Long division P / Q to obtain Φ66(x) (leading coeff of Q is 1) */
    size_t degP = lenP - 1;
    size_t degQ = lenQ - 1;
    size_t degQuot = degP - degQ;               /* should be 20 */
    seq_t quo[200] = {0};

    for (size_t rev = 0; rev <= degQuot; ++rev) {
        size_t i = degQuot - rev;
        seq_t coeff = P[i + degQ];
        quo[i] = coeff;
        for (size_t j = 0; j <= degQ; ++j)
            P[i + j] -= coeff * Q[j];
    }

    /* Copy Φ66(x) coefficients (φ_0 … φ_20) */
    for (size_t i = 0; i <= 20; ++i)
        phi_coeff[i] = quo[i];

    /* Compute the first 66 terms of the sequence via the recurrence */
    cache[0] = 1;
    for (size_t n = 1; n < 66; ++n) {
        seq_t sum = 0;
        size_t kmax = (n < 20) ? n : 20;
        for (size_t k = 1; k <= kmax; ++k)
            sum += phi_coeff[k] * cache[n - k];
        cache[n] = -sum;
    }
}

/* --------- Public function --------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_table();
        initialized = true;
    }
    size_t idx = (size_t)(n % 66);
    return cache[idx];
}
