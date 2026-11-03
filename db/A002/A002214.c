/*
 * A002214  Number of unrooted hexagonal polyominoes with n cells and no
 *          reflections allowed.
 * https://oeis.org/A002214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: x + x*U(x) + (1/2)*(3*x-1)*U(x)^2 + (1/2)*(1+x)*U(x^2) +
 *            (1/3)*x*U(x)^3 + (2/3)*x*U(x^3), U(x) =
 *            (1-3*x-sqrt(1-6*x+5*x^2))/(2*x), i.e. U = x + 3*x*U + x*U^2.
 *            With u(k) = [x^k]U: u(0)=0, u(1)=1, (k+1)*u(k) = 3*(2k-1)*u(k-1)
 *            - 5*(k-2)*u(k-2) for k>=2. [x^k]U^2 = u(k+1) - 3*u(k) - [k=0],
 *            [x^k]...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 33, u128 n <= 61,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 186 (2026-11-03)
 * Helpers:   mem, triangular_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/triangular_seq.h"
#include "../utils/mem.h"

static seq_t a002214_S2(const seq_t *u, idx_t k) {
    seq_t s = 0;
    idx_t i;
    for (i = 1; 2 * i < k; i++) s += u[i] * u[k - i];
    if (k % 2 == 0) s += triangular_seq(u[k / 2]);
    return s;
}

static int a002214_fixedw(void) {
    seq_t t = 1;
    int i;
    for (i = 0; i < 200; i++) t <<= 1;
    return t == 0;
}

/* u(k) via the holonomic recurrence (exact; used for arbitrary precision) */
static seq_t a002214_ub(idx_t k) {
    seq_t b2, b1;
    idx_t j;
    if (k == 0) return 0;
    if (k == 1) return 1;
    b2 = 1; b1 = 1;
    for (j = 2; j <= k; j++) {
        seq_t b = ((seq_t)(6 * j - 3) * b1 - (seq_t)(5 * j - 10) * b2) / (seq_t)(j + 1);
        b2 = b1;
        b1 = b;
    }
    return b1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!a002214_fixedw()) {
        seq_t un1 = a002214_ub(n - 1), un = a002214_ub(n), unp = a002214_ub(n + 1);
        seq_t U2n1 = un - 3 * un1;
        seq_t U2n, U3, h, t, pos, neg;
        if (n == 1) U2n1 -= 1;
        U2n = unp - 3 * un;
        U3 = U2n - 3 * U2n1;
        U3 -= un1;
        h = (n % 2 == 0) ? a002214_ub(n / 2) : a002214_ub((n - 1) / 2);
        t = ((n - 1) % 3 == 0) ? a002214_ub((n - 1) / 3) : (seq_t)0;
        pos = 6 * un1 + 9 * U2n1 + 3 * h + 2 * U3 + 4 * t;
        if (n == 1) pos += 6;
        neg = 3 * U2n;
        return (pos - neg) / 6;
    } else {
        /* fixed width: ring operations only (wraparound-safe), division by 3 via inverse */
        idx_t L = n + 2, k, i;
        seq_t *u = mem_new(seq_t, (size_t)L);
        seq_t U2n1, U2n, U3, t, C3, inv3, res;
        int r;
        u[0] = 0; u[1] = 1;
        for (k = 2; k < L; k++) {
            seq_t s = (seq_t)3 * u[k - 1];
            for (i = 1; i + 1 < k; i++) s += u[i] * u[k - 1 - i];
            u[k] = s;
        }
        U2n1 = u[n] - (seq_t)3 * u[n - 1];
        if (n == 1) U2n1 -= 1;
        U2n = u[n + 1] - (seq_t)3 * u[n];
        U3 = U2n - (seq_t)3 * U2n1 - u[n - 1];
        t = ((n - 1) % 3 == 0) ? u[(n - 1) / 3] : (seq_t)0;
        inv3 = 3;
        for (r = 0; r < 8; r++) inv3 = (seq_t)(inv3 * (seq_t)((seq_t)2 - (seq_t)3 * inv3));
        C3 = (seq_t)((seq_t)(U3 + (seq_t)2 * t) * inv3);
        res = u[n - 1] + U2n1 + a002214_S2(u, n - 1) + C3;
        if (n == 1) res += 1;
        if (n % 2 == 0) res += u[n / 2];
        res -= a002214_S2(u, n);
        mem_free(u);
        return res;
    }
}
