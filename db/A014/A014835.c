/*
 * A014835  Inverse of 826th cyclotomic polynomial.
 * https://oeis.org/A014835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1 / Φ_826(x), where Φ_826 is the 826‑th cyclotomic polynomial.
 *            The sequence is periodic with period 826
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define N          826ULL            /* the cyclotomic index */
#define DEG        348U              /* φ(826) = 348 */

/* Storage for the coefficients of Φ_826(x).  The polynomial is monic,
   so phi_coeff[0] = 1.  The indices run from 0 to DEG. */
static seq_t   phi_coeff[DEG + 1] = {0};
static bool    phi_ready = false;

/* Storage for the first 826 terms of the reciprocal series 1/Φ_826(x). */
static seq_t   a_period[826] = {0};
static bool    a_ready   = false;

/* Compute the coefficients of Φ_826(x). */
static void compute_phi_826(void)
{
    if (phi_ready) return;

    /* List all positive divisors of N.  N is small, so a simple loop suffices. */
    uint64_t divs[64];
    size_t   nd = 0;
    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            divs[nd++] = d;
            if (d != N / d) divs[nd++] = N / d;
        }
    }

    /* Two working polynomials: one for the product with μ=+1, one for μ=-1. */
    seq_t num[2000] = {0};
    seq_t den[2000] = {0};
    size_t deg_num = 0;   /* current degree of num   */
    size_t deg_den = 0;   /* current degree of den   */

    num[0] = 1;   /* start with the polynomial 1 */
    den[0] = 1;

    for (size_t idx = 0; idx < nd; ++idx) {
        uint64_t d  = divs[idx];
        uint64_t e  = N / d;
        int      mu = 0;   /* Möbius of e */

        /* Compute μ(e) by factorisation of the small integer e. */
        uint64_t tmp   = e;
        bool     square = false;
        unsigned pcnt   = 0;              /* number of distinct primes */
        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                int cnt = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++cnt;
                    if (cnt > 1) { square = true; break; }
                }
                if (square) break;
                ++pcnt;
            }
        }
        if (!square && tmp > 1) ++pcnt;
        if (square)           mu = 0;
        else                  mu = (pcnt & 1) ? -1 : 1;

        if (mu == 0) continue;    /* skip when μ(e) = 0 */

        if (mu > 0) {
            /* Multiply num by (x^d – 1). */
            size_t new_deg = deg_num + d;
            seq_t tmp_poly[2000] = {0};
            for (size_t i = 0; i <= new_deg; ++i) tmp_poly[i] = 0;
            for (size_t i = 0; i <= deg_num; ++i) {
                tmp_poly[i]       -= num[i];   /* -1 part   */
                tmp_poly[i + d]   += num[i];   /* x^d part  */
            }
            deg_num = new_deg;
            for (size_t i = 0; i <= deg_num; ++i) num[i] = tmp_poly[i];
        } else { /* mu < 0 */
            /* Multiply den by (x^d – 1). */
            size_t new_deg = deg_den + d;
            seq_t tmp_poly[2000] = {0};
            for (size_t i = 0; i <= new_deg; ++i) tmp_poly[i] = 0;
            for (size_t i = 0; i <= deg_den; ++i) {
                tmp_poly[i]       -= den[i];
                tmp_poly[i + d]   += den[i];
            }
            deg_den = new_deg;
            for (size_t i = 0; i <= deg_den; ++i) den[i] = tmp_poly[i];
        }
    }

    /* Polynomial division:  num / den  yields Φ_826.  Den has leading
       coefficient 1, so ordinary long division is sufficient. */
    seq_t tmp[2000];
    for (size_t i = 0; i <= deg_num; ++i) tmp[i] = num[i];

    size_t qdeg = deg_num - deg_den;   /* should equal DEG (348) */
    for (size_t i = 0; i <= qdeg; ++i) phi_coeff[i] = 0;

    for (int64_t i = (int64_t)deg_num; i >= (int64_t)deg_den; --i) {
        seq_t coeff = tmp[i];
        if (coeff == 0) continue;
        phi_coeff[i - deg_den] = coeff;
        for (size_t j = 0; j <= deg_den; ++j) {
            tmp[i - deg_den + j] -= coeff * den[j];
        }
    }

    phi_ready = true;
}

/* Compute the first 826 terms of 1/Φ_826(x) via the recurrence. */
static void compute_a_period(void)
{
    if (a_ready) return;
    compute_phi_826();

    a_period[0] = (seq_t)1;
    for (uint64_t n = 1; n < 826u; ++n) {
        seq_t acc = 0;
        size_t kmax = (n <= (uint64_t)DEG) ? (size_t)n : DEG;
        for (size_t k = 1; k <= kmax; ++k) {
            acc += phi_coeff[k] * a_period[n - k];
        }
        a_period[n] = -acc;
    }
    a_ready = true;
}

seq_t a(idx_t n)
{
    compute_a_period();
    uint64_t pos = (uint64_t)n % 826u;
    return a_period[pos];
}
