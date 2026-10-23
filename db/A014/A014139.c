/*
 * A014139  Inverse of 130th cyclotomic polynomial.
 * https://oeis.org/A014139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in 1/Φ₁₃₀(x); * it satisfies
 *            Φ₁₃₀(x) · Σ a(k)xᵏ = 1, giving a periodic * sequence of period
 *            130
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAXN 131                     /* 0 … 130 */
#define MAXLEN MAXN

static seq_t poly[MAXN][MAXLEN];
static int poly_len[MAXN];
static unsigned char poly_computed[MAXN] = {0};

/* Return all proper divisors of n (excluding n itself).  The order is sorted. */
static void get_divisors(int n, int *out, int *cnt) {
    int c = 0;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            int d1 = i;
            int d2 = n / i;
            if (d1 != n) out[c++] = d1;
            if (d2 != d1 && d2 != n) out[c++] = d2;
        }
    }
    *cnt = c;
    /* simple bubble sort for determinism (c <= 8 for n = 130) */
    for (int i = 0; i < c; ++i)
        for (int j = i + 1; j < c; ++j)
            if (out[i] > out[j]) {
                int t = out[i];
                out[i] = out[j];
                out[j] = t;
            }
}

/* Compute Φ_n(x) in poly[n] (coefficients c₀ … c_d, c_d = 1). */
static void compute_phi(int n) {
    if (poly_computed[n]) return;

    /* Start with xⁿ – 1 */
    for (int i = 0; i <= n; ++i) poly[n][i] = 0;
    poly[n][0] = -1;
    poly[n][n] = 1;
    poly_len[n] = n + 1;

    int divs[64], dcnt;
    get_divisors(n, divs, &dcnt);
    for (int idx = 0; idx < dcnt; ++idx) {
        int d = divs[idx];
        compute_phi(d);

        int len_n = poly_len[n];
        int len_d = poly_len[d];
        int qlen  = len_n - len_d + 1;

        seq_t tmp[MAXLEN];
        seq_t q[MAXLEN];
        for (int i = 0; i < len_n; ++i) tmp[i] = poly[n][i];
        for (int i = 0; i < qlen; ++i) q[i] = 0;

        for (int i = len_n - 1; i >= len_d - 1; --i) {
            seq_t coeff = tmp[i];
            int qpos = i - (len_d - 1);
            q[qpos] = coeff;
            for (int j = 0; j < len_d; ++j)
                tmp[qpos + j] -= coeff * poly[d][j];
        }
        for (int i = 0; i < qlen; ++i) poly[n][i] = q[i];
        poly_len[n] = qlen;
    }
    poly_computed[n] = 1;
}

/* Sequence function */
seq_t a(idx_t n) {
    static unsigned char init = 0;
    static seq_t a_cache[130];
    static seq_t phi_coef[49];
    static int phi_deg = 0;

    if (!init) {
        /* Compute Φ₁₃₀(x) */
        compute_phi(130);
        phi_deg = poly_len[130] - 1;
        for (int i = 0; i <= phi_deg; ++i) phi_coef[i] = poly[130][i];

        /* First 130 terms via the recurrence */
        a_cache[0] = 1;
        for (int i = 1; i < 130; ++i) {
            seq_t s = 0;
            int kmax = i < phi_deg ? i : phi_deg;
            for (int k = 1; k <= kmax; ++k)
                s += phi_coef[k] * a_cache[i - k];
            a_cache[i] = -s;
        }
        init = 1;
    }
    return a_cache[(size_t)(n % 130)];
}
