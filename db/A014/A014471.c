/*
 * A014471  Inverse of 462nd cyclotomic polynomial.
 * https://oeis.org/A014471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   poly_div_inplace
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the formal power series expansion of 1/Φ₄₆₂(x).
 * Recurrence: a(0) = 1
 * a(n) = -∑_{k=1}^{min(n,φ(462))} Φ₄₆₂[k] * a(n-k)   (periodic with period 462). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/poly_div_inplace.h"

/* Return all divisors of n in ascending order in divs[64];
 * nd receives the number of divisors. */
static void get_divisors(idx_t n, idx_t divs[64], size_t *nd)
{
    size_t count = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[count++] = i;
            if (i != n / i) divs[count++] = n / i;
        }
    }
    /* simple insertion sort */
    for (size_t i = 1; i < count; ++i) {
        idx_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }
    *nd = count;
}

/* Compute Φ_d(x) for each divisor d of n, storing coefficients in phi[d]
 * and lengths in phi_len[d].  Each polynomial is represented in
 * coefficient‑by‑constant order. */
static void compute_phi_divisors(idx_t n,
                                 seq_t phi[][121], size_t phi_len[463])
{
    idx_t divs[64];
    size_t nd;
    get_divisors(n, divs, &nd);

    for (size_t pos = 0; pos < nd; ++pos) {
        idx_t d = divs[pos];
        if (d == 1) {
            phi_len[1] = 2;
            phi[1][0] = -1;
            phi[1][1] = 1;
            continue;
        }

        seq_t tmp[463];
        size_t len_f = d + 1;
        for (size_t i = 0; i < len_f; ++i) tmp[i] = 0;
        tmp[0] = -1;
        tmp[d] = 1;                       /* -1 + x^d */

        for (size_t j = 0; j < pos; ++j) {
            idx_t dp = divs[j];
            if (d % dp == 0) {
                poly_div_inplace(tmp, &len_f, phi[dp], phi_len[dp]);
            }
        }

        phi_len[d] = len_f;
        for (size_t i = 0; i < len_f; ++i) phi[d][i] = tmp[i];
    }
}

static bool initialized = false;
static seq_t period[462];

static void init_sequence(void)
{
    seq_t phi[463][121];
    size_t phi_len[463];
    compute_phi_divisors(462, phi, phi_len);

    size_t deg = phi_len[462] - 1;   /* deg = φ(462) = 120 */

    period[0] = 1;
    for (idx_t n = 1; n < 462; ++n) {
        seq_t val = 0;
        size_t kmax = (n < deg) ? n : deg;
        for (size_t k = 1; k <= kmax; ++k) {
            val -= phi[462][k] * period[n - k];
        }
        period[n] = val;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    return period[(size_t)(n % 462)];
}
