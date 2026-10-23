/*
 * A015725  Inverse of 1716th cyclotomic polynomial.
 * https://oeis.org/A015725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = coefficients of 1/Φ_1716(x), periodic with period 1716 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------- */
/* Mobius function for m ≤ 1716.                                 */
/* ------------------------------------------------------------- */
static int mobius_uint64(idx_t m)
{
    if (m == 1) return 1;
    int cnt = 0;
    idx_t t = m;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if (e > 1) return 0;           /* squared prime factor */
            ++cnt;
        }
    }
    if (t > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

/* ------------------------------------------------------------- */
/* Compute the coefficients of Φ_1716(x).                         */
/* The result is stored in *phi and its degree is written to *@phi_deg. */
/* ------------------------------------------------------------- */
static void compute_phi(seq_t **phi, size_t *phi_deg)
{
    const idx_t N = 1716ULL;

    /* Gather all divisors of N */
    idx_t divs[240];
    size_t nd = 0;
    for (idx_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            divs[nd++] = d;
            if (d != N / d) divs[nd++] = N / d;
        }
    }

    /* Separate divisors according to μ(N/d) */
    idx_t pos_ds[240], neg_ds[240];
    size_t np = 0, nn = 0;
    for (size_t i = 0; i < nd; ++i) {
        idx_t d = divs[i];
        idx_t m = N / d;
        int mu = mobius_uint64(m);
        if (mu == 1)      pos_ds[np++] = d;
        else if (mu == -1)neg_ds[nn++] = d;
    }

    /* Build the numerator N = ∏(x^d-1) for μ=1 */
    seq_t *poly = (seq_t*)calloc(1, sizeof(seq_t));
    poly[0] = 1;
    size_t deg = 0;
    for (size_t i = 0; i < np; ++i) {
        idx_t d = pos_ds[i];
        size_t new_deg = deg + d;
        seq_t *new_poly = (seq_t*)calloc(new_deg + 1, sizeof(seq_t));
        for (size_t k = 0; k <= deg; ++k) {
            new_poly[k] += poly[k];
            new_poly[k + d] -= poly[k];
        }
        free(poly);
        poly = new_poly;
        deg = new_deg;
    }

    /* Divide by D = ∏(x^d-1) for μ=-1, one factor at a time */
    for (size_t i = 0; i < nn; ++i) {
        idx_t d = neg_ds[i];
        size_t new_deg = deg - d;
        seq_t *new_poly = (seq_t*)calloc(new_deg + 1, sizeof(seq_t));
        for (size_t m = 0; m <= new_deg; ++m) {
            if (m < d) {
                new_poly[m] = -poly[m];
            } else {
                new_poly[m] = new_poly[m - d] - poly[m];
            }
        }
        free(poly);
        poly = new_poly;
        deg = new_deg;
    }

    *phi      = poly;
    *phi_deg  = deg;
}

/* ------------------------------------------------------------- */
/* Public function required by the framework.                    */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static const idx_t PERIOD = 1716ULL;
    static seq_t *phi      = NULL;
    static size_t  phi_deg = 0;
    static seq_t *period   = NULL;

    if (!phi) {
        compute_phi(&phi, &phi_deg);
    }

    if (!period) {
        period = (seq_t*)calloc(PERIOD, sizeof(seq_t));
        period[0] = 1;
        for (idx_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            idx_t maxj = (i < phi_deg) ? i : phi_deg;
            for (idx_t j = 1; j <= maxj; ++j) {
                sum += phi[j] * period[i - j];
            }
            period[i] = -sum;
        }
    }

    return period[n % PERIOD];
}
