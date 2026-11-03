/*
 * A002520  Theta series of 28-dimensional unimodular lattice with no roots
 *          and with no parity vector of norm 4.
 * https://oeis.org/A002520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = [q^n] ( t3^28 - 56*t3^20*D8 + 280*t3^12*D8^2 ), t3 =
 *            theta_3 = Sum_{k in Z} q^(k^2), t4 = theta_4 = Sum_{k in Z}
 *            (-1)^k q^(k^2), D8 = (theta_2*theta_4)^4/16 = q * psi^4 * t4^4
 *            with psi = Sum_{k>=0} q^(k(k+1)). Evaluated as t3^12 * (
 *            t3^8*(t3^8 - 56*D8) + 280*D8^2 ) using sparse series mult...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* f *= psi = Sum_{k>=0} q^(k(k+1)), in place */
static void a2520_mul_psi(seq_t *f, idx_t L) {
    idx_t m, k;
    for (m = L; m-- > 0;) {
        seq_t s = 0;
        for (k = 1; k * (k + 1) <= m; k++) s += f[m - k * (k + 1)];
        f[m] += s;
    }
}

/* f *= theta_4, in place */
static void a2520_mul_t4(seq_t *f, idx_t L) {
    idx_t m, k;
    for (m = L; m-- > 0;) {
        seq_t p = 0, q = 0;
        for (k = 1; k * k <= m; k++) {
            if (k & 1) q += f[m - k * k];
            else p += f[m - k * k];
        }
        f[m] += 2 * p;
        f[m] -= 2 * q;
    }
}

/* f *= D8 = q * psi^4 * theta_4^4, in place */
static void a2520_mul_D8(seq_t *f, idx_t L) {
    idx_t m;
    int i;
    for (m = L; m-- > 1;) f[m] = f[m - 1];
    if (L > 0) f[0] = 0;
    for (i = 0; i < 4; i++) a2520_mul_psi(f, L);
    for (i = 0; i < 4; i++) a2520_mul_t4(f, L);
}

/* f *= theta_3, in place, coefficients 0..L-1 */
static void a2520_mul_t3(seq_t *f, idx_t L) {
    idx_t m, k;
    for (m = L; m-- > 0;) {
        seq_t s = 0;
        for (k = 1; k * k <= m; k++) s += f[m - k * k];
        f[m] += 2 * s;
    }
}

static seq_t *a2520_cache = NULL;
static idx_t a2520_len = 0;

static void a2520_build(idx_t L) {
    seq_t *Y = mem_new(seq_t, L);
    seq_t *D = mem_new(seq_t, L);
    idx_t i;
    int r;
    for (i = 0; i < L; i++) { Y[i] = 0; D[i] = 0; }
    Y[0] = 1;
    D[0] = 1;
    for (r = 0; r < 8; r++) a2520_mul_t3(Y, L);      /* Y = t3^8 */
    a2520_mul_D8(D, L);                              /* D = D8 */
    for (i = 0; i < L; i++) Y[i] -= 56 * D[i];       /* Y = t3^8 - 56 D8 */
    for (r = 0; r < 8; r++) a2520_mul_t3(Y, L);      /* Y = t3^16 - 56 t3^8 D8 */
    a2520_mul_D8(D, L);                              /* D = D8^2 */
    for (i = 0; i < L; i++) Y[i] += 280 * D[i];
    for (r = 0; r < 12; r++) a2520_mul_t3(Y, L);     /* times t3^12 */
    mem_free(D);
    if (a2520_cache) mem_free(a2520_cache);
    a2520_cache = Y;
    a2520_len = L;
}

seq_t a(idx_t n) {
    if (n >= a2520_len) {
        idx_t L = n + 1;
        idx_t g = a2520_len + a2520_len / 2;
        if (g > L) L = g;
        if (L < 32) L = 32;
        a2520_build(L);
    }
    return a2520_cache[n];
}
