/*
 * A003450  Number of nonequivalent dissections of an n-gon into n-4 polygons
 *          by nonintersecting diagonals up to rotation and reflection.
 * https://oeis.org/A003450
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over the dihedral group D_n, with a(n) = ( D + R2 + R5
 *            + Refl ) / (2n), where D = (n-3)/2 * binomial(2n-6, n-5) (all
 *            (n-5)-diagonal dissections), R2 = [n even] (n/2) * binomial(n-3,
 *            n/2-3) (fixed by rotation of order 2), R5 = [5|n] 4 *
 *            binomial(2k-2, k-1), k = n/5 (fixed by rotations of...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 188 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a3450_add(uint32_t *s, const uint32_t *t, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t v = (uint64_t)s[i] + t[i] + c;
        s[i] = (uint32_t)v;
        c = v >> 32;
    }
}

static void a3450_binom(uint32_t *x, size_t L, long long N, long long k) {
    long long i;
    size_t top = 1, j;
    memset(x, 0, L * sizeof(uint32_t));
    if (k < 0 || N < k) return;
    x[0] = 1;
    for (i = 1; i <= k; i++) {
        uint64_t c = 0, r = 0;
        uint32_t m = (uint32_t)(N - k + i), d = (uint32_t)i;
        for (j = 0; j < top; j++) {
            uint64_t v = (uint64_t)x[j] * m + c;
            x[j] = (uint32_t)v;
            c = v >> 32;
        }
        if (c && top < L) x[top++] = (uint32_t)c;
        j = top;
        while (j-- > 0) {
            uint64_t v = (r << 32) | x[j];
            x[j] = (uint32_t)(v / d);
            r = v % d;
        }
        while (top > 1 && x[top - 1] == 0) top--;
    }
}

static void a3450_div(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t v = (r << 32) | x[i];
        x[i] = (uint32_t)(v / d);
        r = v % d;
    }
}

static void a3450_mul(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)v;
        c = v >> 32;
    }
}

static void a3450_ballot(uint32_t *x, size_t L, long long m, long long k) {
    if (m < k) { memset(x, 0, L * sizeof(uint32_t)); return; }
    a3450_binom(x, L, 2 * m - k, m - k);
    a3450_mul(x, L, (uint32_t)k);
    a3450_div(x, L, (uint32_t)(2 * m - k));
}

static int a3450_width(void) {
    static int w = -1;
    if (w < 0) {
        seq_t x = 1;
        int c = 0;
        while (x != 0 && c < 400) { x = x * 2; c++; }
        w = c;
    }
    return w;
}

seq_t a(idx_t n) {
    long long N = (long long)n;
    size_t L = (size_t)((2 * N + 160) / 32 + 4);
    size_t i;
    int w = a3450_width();
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *U = (uint32_t *)calloc(L, sizeof(uint32_t));
    seq_t res = 0;

    /* identity */
    a3450_binom(T, L, 2 * N - 6, N - 5);
    a3450_mul(T, L, (uint32_t)(N - 3));
    a3450_div(T, L, 2);
    a3450_add(S, T, L);

    /* rotation by half-turn */
    if (N % 2 == 0 && N / 2 >= 3) {
        a3450_binom(T, L, N - 3, N / 2 - 3);
        a3450_mul(T, L, (uint32_t)(N / 2));
        a3450_add(S, T, L);
    }
    /* rotations of order 5 */
    if (N % 5 == 0) {
        long long k = N / 5;
        a3450_binom(T, L, 2 * k - 2, k - 1);
        a3450_mul(T, L, 4);
        a3450_add(S, T, L);
    }

    /* reflections */
    if (N % 2 == 1) {
        long long m = (N - 1) / 2;
        a3450_ballot(T, L, m, 2); a3450_add(U, T, L);
        a3450_ballot(T, L, m, 3); a3450_add(U, T, L);
        a3450_binom(T, L, 2 * m - 3, m - 3); a3450_add(U, T, L);
        a3450_mul(U, L, (uint32_t)N);
    } else {
        long long m = N / 2;
        a3450_binom(T, L, 2 * m - 3, m - 3); a3450_add(U, T, L);
        a3450_ballot(T, L, m, 3); a3450_mul(T, L, 2); a3450_add(U, T, L);
        a3450_ballot(T, L, m, 4); a3450_add(U, T, L);
        a3450_binom(T, L, 2 * m - 4, m - 4); a3450_mul(T, L, 2); a3450_add(U, T, L);
        a3450_ballot(T, L, m - 1, 2); a3450_add(U, T, L);
        a3450_mul(U, L, (uint32_t)m);
    }
    a3450_add(S, U, L);

    a3450_div(S, L, (uint32_t)(2 * N));

    for (i = 0; i < L; i++) {
        if (w < 400 && (long long)(32 * i) >= (long long)w) break;
        if (S[i]) res += ((seq_t)S[i]) << (idx_t)(32 * i);
    }
    free(S);
    free(T);
    free(U);
    return res;
}
