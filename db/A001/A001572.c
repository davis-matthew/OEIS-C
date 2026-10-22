/*
 * A001572  Related to series-parallel networks.
 * https://oeis.org/A001572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000084(n) - Sum_{k=1..n-1} a(k)*A000084(n-k) for n >= 1,
 *            a(0) = 1 (INVERTi transform of A000084). A000084 = Euler
 *            transform of A000669, where A000669(1) = 1 and A000669(n) =
 *            A000084(n)/2 for n >= 2. With c(k) = Sum_{d|k} d*A000669(d),
 *            A000669(n) = (Sum_{k=1..n-1} c(k)*A000084(n-k) + Sum_{...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   a1572_fixed, a1572_regrow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a1572_fixed.h"
#include "../utils/a1572_regrow.h"

/* ---------- fixed-width path: arithmetic mod 2^128 ---------- */
typedef unsigned __int128 a1572_u;
static a1572_u *F_s = NULL, *F_b = NULL, *F_c = NULL, *F_a = NULL;
static idx_t F_len = 0, F_cap = 0;

static a1572_u a1572_inv(a1572_u m) {
    a1572_u x = m;
    int i;
    for (i = 0; i < 7; i++) x *= (a1572_u)2 - m * x;
    return x;
}

static void a1572_fixed_extend(idx_t n) {
    idx_t i, k;
    if (n >= F_cap) {
        idx_t nc = F_cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        F_s = (a1572_u *)realloc(F_s, nc * sizeof(a1572_u));
        F_b = (a1572_u *)realloc(F_b, nc * sizeof(a1572_u));
        F_c = (a1572_u *)realloc(F_c, nc * sizeof(a1572_u));
        F_a = (a1572_u *)realloc(F_a, nc * sizeof(a1572_u));
        F_cap = nc;
    }
    if (F_len == 0) {
        F_s[0] = 0; F_b[0] = 0; F_c[0] = 0; F_a[0] = 1;
        F_s[1] = 1; F_b[1] = 1; F_c[1] = 1; F_a[1] = 1;
        F_len = 2;
    }
    for (i = F_len; i <= n; i++) {
        a1572_u R = 0, D = 0, T, S = 0;
        idx_t m = i;
        int v = 0;
        for (k = 1; k < i; k++) R += F_c[k] * F_b[i - k];
        for (k = 1; k <= i / 2; k++)
            if (i % k == 0) D += (a1572_u)k * F_s[k];
        T = R + D;
        while ((m & 1) == 0) { m >>= 1; v++; }
        F_s[i] = (T >> v) * a1572_inv((a1572_u)m);
        F_c[i] = D + (a1572_u)i * F_s[i];
        F_b[i] = 2 * F_s[i];
        for (k = 1; k < i; k++) S += F_a[k] * F_b[i - k];
        F_a[i] = F_b[i] - S;
    }
    F_len = n + 1;
}

/* ---------- arbitrary-precision path ---------- */
static seq_t *B_s = NULL, *B_b = NULL, *B_c = NULL, *B_a = NULL;
static idx_t B_len = 0, B_cap = 0;

static void a1572_big_extend(idx_t n) {
    idx_t i, k;
    if (n >= B_cap) {
        idx_t nc = B_cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        B_s = a1572_regrow(B_s, B_len, nc);
        B_b = a1572_regrow(B_b, B_len, nc);
        B_c = a1572_regrow(B_c, B_len, nc);
        B_a = a1572_regrow(B_a, B_len, nc);
        B_cap = nc;
    }
    if (B_len == 0) {
        B_s[0] = 0; B_b[0] = 0; B_c[0] = 0; B_a[0] = 1;
        B_s[1] = 1; B_b[1] = 1; B_c[1] = 1; B_a[1] = 1;
        B_len = 2;
    }
    for (i = B_len; i <= n; i++) {
        seq_t R = 0, D = 0, S = 0;
        for (k = 1; k < i; k++) R += B_c[k] * B_b[i - k];
        for (k = 1; k <= i / 2; k++)
            if (i % k == 0) D += (seq_t)k * B_s[k];
        B_s[i] = (R + D) / (seq_t)i;
        B_c[i] = D + (seq_t)i * B_s[i];
        B_b[i] = B_s[i] * 2;
        for (k = 1; k < i; k++) S += B_a[k] * B_b[i - k];
        B_a[i] = B_b[i] - S;
    }
    B_len = n + 1;
}

seq_t a(idx_t n) {
    static int mode = -1;
    if (mode < 0) mode = a1572_fixed();
    if (n == 0) return 1;
    if (mode) {
        a1572_u v;
        seq_t r;
        int i;
        if (n >= F_len) a1572_fixed_extend(n);
        v = F_a[n];
        r = (seq_t)(uint64_t)(v >> 64);
        for (i = 0; i < 8; i++) r = r * (seq_t)256;
        r = r + (seq_t)(uint64_t)v;
        return r;
    } else {
        if (n >= B_len) a1572_big_extend(n);
        return B_a[n];
    }
}
