/*
 * A000648  Number of alkyls C_{n+15} H_{2n+10} (Anthr.) with n carbon atoms.
 * https://oeis.org/A000648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. A(x) = (r(x)^10 + r(x)^2*r(x^2)^4 + 2*r(x^2)^5)/4, r(x) =
 *            A000598(x), with r(n+1) = ([x^n] r^3 + 3*[x^n] r(x)r(x^2) +
 *            2*[x^n] r(x^3))/6, r(0) = 1. Writing D = r(x^2) and E = (r(x)^2
 *            - r(x^2))/2 (unordered pairs of distinct radicals), this is A =
 *            D^5 + 3*D^4*E + 10*D^3*E^2 + 20*D^2*E^3 + 20*D...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_zero, ch2_s, mul_tr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/ch2_s.h"
#include "../utils/mul_tr.h"

static seq_t *A_tab = NULL;
static idx_t A_cap = 0;

static void build(idx_t N) {
    seq_t *r = alloc_zero(N);
    seq_t *R2 = alloc_zero(N);
    seq_t *R3 = alloc_zero(N);
    seq_t *D = alloc_zero(N);
    seq_t *E = alloc_zero(N);
    seq_t *D2 = alloc_zero(N);
    seq_t *D3 = alloc_zero(N);
    seq_t *D4 = alloc_zero(N);
    seq_t *D5 = alloc_zero(N);
    seq_t *T = alloc_zero(N);
    seq_t *U = alloc_zero(N);
    idx_t m, i;

    r[0] = 1; R2[0] = 1; R3[0] = 1;
    for (m = 1; m < N; m++) {
        idx_t n = m - 1;
        seq_t s = R3[n];
        seq_t t = 0;
        for (i = 0; 2 * i <= n; i++) t += r[n - 2 * i] * r[i];
        s += (seq_t)3 * t;
        if (n % 3 == 0) s += (seq_t)2 * r[n / 3];
        r[m] = s / 6;
        {
            seq_t q = 0;
            for (i = 0; i <= m; i++) q += r[i] * r[m - i];
            R2[m] = q;
            q = 0;
            for (i = 0; i <= m; i++) q += r[i] * R2[m - i];
            R3[m] = q;
        }
    }

    for (m = 0; m < N; m++) {
        seq_t e = 0;
        if (m % 2 == 0) D[m] = r[m / 2];
        for (i = 0; 2 * i < m; i++) e += r[i] * r[m - i];
        if (m % 2 == 0) e += ch2_s(r[m / 2]);
        E[m] = e;
    }

    mul_tr(D, D, D2, N);
    mul_tr(D, D2, D3, N);
    mul_tr(D, D3, D4, N);
    mul_tr(D, D4, D5, N);

    for (m = 0; m < N; m++) T[m] = (seq_t)20 * D[m] + (seq_t)8 * E[m];
    mul_tr(E, T, U, N);
    for (m = 0; m < N; m++) T[m] = U[m] + (seq_t)20 * D2[m];
    mul_tr(E, T, U, N);
    for (m = 0; m < N; m++) T[m] = U[m] + (seq_t)10 * D3[m];
    mul_tr(E, T, U, N);
    for (m = 0; m < N; m++) T[m] = U[m] + (seq_t)3 * D4[m];
    mul_tr(E, T, U, N);
    for (m = 0; m < N; m++) U[m] += D5[m];

    A_tab = U;
    A_cap = N;
}

seq_t a(idx_t n) {
    if (n >= A_cap) {
        idx_t nc = A_cap + A_cap / 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 32) nc = 32;
        build(nc);
    }
    return A_tab[n];
}
