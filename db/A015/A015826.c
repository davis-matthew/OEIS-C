/*
 * A015826  Inverse of 1817th cyclotomic polynomial.
 * https://oeis.org/A015826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the coefficients of the inverse power series of the 1817th cyclotomic
 * polynomial Φ₁₈₁₇(x).  The sequence is periodic with period 1817, so
 * a(n) depends only on n mod 1817. This implementation precomputes the
 * entire period once and returns a(n) for arbitrary n. */
#include <stddef.h>
#include <stdbool.h>

/* Precomputed data for the period.  The static array is filled once by
 * init_a() on the first call to a().  The cache holds 1817 coefficients. */
static seq_t a_cache[1817];
static bool a_initialized = false;

/* Polynomial multiplication: out = p * q.  All arrays are assumed to
 * have lengths plen, qlen, outlen = plen + qlen - 1. */
static void poly_mul(const seq_t *p, size_t plen,
                     const seq_t *q, size_t qlen,
                     seq_t *out)
{
    size_t outlen = plen + qlen - 1;
    for (size_t i = 0; i < outlen; ++i) out[i] = 0;
    for (size_t i = 0; i < plen; ++i)
        for (size_t j = 0; j < qlen; ++j)
            out[i + j] += p[i] * q[j];
}

/* Initialise the period cache by computing the coefficients of Φ₁₈₁₇(x)
 * and then the inverse power‑series coefficients a(n). */
static void init_a(void)
{
    /* φ₁₈₁₇(x) = Φ₁₈₁₇(x) = (x^1817 – 1)*(x – 1) /
                        (x^23 – 1)*(x^79 – 1)          */
    /* Build (x – 1)*(1 + x + … + x^22) = x^23 – 1. */
    seq_t phi1[2] = { -1, 1 };            /* x – 1          */
    seq_t phi23[23];
    for (size_t i = 0; i < 23; ++i) phi23[i] = 1;   /* 1 + … + x^22   */
    seq_t temp[24];                     /* degree 23        */
    poly_mul(phi1, 2, phi23, 23, temp);            /* φ₁₈₁₇‑temp1 = x^23 – 1 */

    /* Build (x^23 – 1)*(1 + x + … + x^78) = (x^23 – 1)*(x^79 – 1)/(x – 1). */
    seq_t phi79[79];
    for (size_t i = 0; i < 79; ++i) phi79[i] = 1;   /* 1 + … + x^78   */
    seq_t pi[102];                    /* degree 101       */
    poly_mul(temp, 24, phi79, 79, pi);             /* π(x) = (x^23 – 1)*(x^79 – 1)/(x – 1) */

    /* Numerator N(x) = x^1817 – 1. */
    seq_t N[1818];
    for (size_t i = 0; i < 1818; ++i) N[i] = 0;
    N[0] = -1;
    N[1817] = 1;

    /* We will divide N by π to obtain Φ₁₈₁₇(x).  use C as a mutable copy  */
    seq_t C[1818];
    for (size_t i = 0; i < 1818; ++i) C[i] = N[i];

    /* Resulting coefficients of Φ₁₈₁₇: degree 1716. */
    seq_t phi1817[1717];
    for (size_t i = 0; i < 1717; ++i) phi1817[i] = 0;

    size_t degP = 101;          /* degree of π   */
    size_t degN = 1817;         /* degree of N   */

    for (size_t k = degN; k >= degP; --k) {
        seq_t factor = C[k];           /* leading coeff of π is 1 */
        size_t qidx = k - degP;
        phi1817[qidx] = factor;
        if (factor != 0) {
            for (size_t j = 0; j <= degP; ++j)
                C[qidx + j] -= factor * pi[j];
        }
    }

    /* Compute the inverse power‑series coefficients a(n).  The recurrence is
     * a_0 = 1, a_n = –∑_{k=1}^{min(n,1716)} c_k * a_{n–k}, where c_k
     * are the coefficients of Φ₁₈₁₇(x). */
    a_cache[0] = 1;                     /* a(0) = 1 */
    for (size_t n = 1; n < 1817; ++n) {
        seq_t sum = 0;
        size_t maxk = (n <= 1716) ? n : 1716;
        for (size_t k = 1; k <= maxk; ++k)
            sum += phi1817[k] * a_cache[n - k];
        a_cache[n] = -sum;
    }
}

/* Public API: a(n) returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (!a_initialized) {
        init_a();
        a_initialized = true;
    }
    idx_t mod = n % 1817ULL;
    return a_cache[(size_t)mod];
}
