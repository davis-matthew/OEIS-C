/*
 * A001559  a(0) = 1, a(1) = 4; thereafter a(n)*(2n + 10) - a(n-1)*(11n + 35)
 *          + a(n-2)*(8n + 2) + a(n-3)*(15n + 7) + a(n-4)*(4n - 2) = 0.
 * https://oeis.org/A001559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n*(2n+10)*(7n+13)*a(n) = (49n^3+252n^2+419n+240)*a(n-1) +
 *            (2n+2)*(2n+3)*(7n+20)*a(n-2), a(0) = 1, a(1) = 4. For
 *            fixed-width seq_t: G.f. 1/E(x), E(x) = 1 - 3x + x^3 -
 *            (1-2x-x^2)*T(x), T(x) = Sum_{k>=1} C(k-1) x^k (Catalan), so a(n)
 *            = -Sum_{k=1..n} E_k a(n-k), computed exactly mod 2^128
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   a001559_fixed_mode, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a001559_fixed_mode.h"

static seq_t a001559_fixed(idx_t n) {
    typedef unsigned __int128 u128;
    idx_t k, j;
    u128 *C, *T, *E, *A, s, r;
    if (n > 1000) return 0; /* cannot fit in any fixed-width type */
    C = (u128 *)malloc((n + 2) * sizeof(u128));
    T = (u128 *)malloc((n + 2) * sizeof(u128));
    E = (u128 *)malloc((n + 2) * sizeof(u128));
    A = (u128 *)malloc((n + 2) * sizeof(u128));
    C[0] = 1;
    for (k = 1; k <= n; k++) {
        s = 0;
        for (j = 0; j < k; j++) s += C[j] * C[k - 1 - j];
        C[k] = s;
    }
    T[0] = 0;
    for (k = 1; k <= n; k++) T[k] = C[k - 1];
    for (k = 0; k <= n; k++) {
        u128 e = 0;
        if (k == 0) e += 1;
        if (k == 1) e -= 3;
        if (k == 3) e += 1;
        e -= T[k];
        if (k >= 1) e += 2 * T[k - 1];
        if (k >= 2) e += T[k - 2];
        E[k] = e;
    }
    A[0] = 1;
    for (k = 1; k <= n; k++) {
        s = 0;
        for (j = 1; j <= k; j++) s += E[j] * A[k - j];
        A[k] = (u128)0 - s;
    }
    r = A[n];
    free(C); free(T); free(E); free(A);
    return u128_to_seq(r);
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t filled = 0;
    static int mode = -1;
    idx_t i;

    if (mode < 0) mode = a001559_fixed_mode();
    if (mode) return a001559_fixed(n);

    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (i = 0; i < cap; i++) cache[i] = 0;
        cache[0] = 1;
        cache[1] = 4;
        filled = 2;
    }
    if (n < filled) return cache[n];
    if (n >= cap) {
        idx_t nc = cap;
        seq_t *nb;
        while (nc <= n) nc *= 2;
        nb = (seq_t *)malloc(nc * sizeof(seq_t));
        for (i = 0; i < nc; i++) nb[i] = 0;
        for (i = 0; i < filled; i++) nb[i] = cache[i];
        free(cache);
        cache = nb;
        cap = nc;
    }
    for (i = filled; i <= n; i++) {
        seq_t m = (seq_t)i;
        seq_t c1 = (seq_t)49 * m * m * m + (seq_t)252 * m * m + (seq_t)419 * m + (seq_t)240;
        seq_t c2 = ((seq_t)2 * m + (seq_t)2) * ((seq_t)2 * m + (seq_t)3) * ((seq_t)7 * m + (seq_t)20);
        seq_t d = m * ((seq_t)2 * m + (seq_t)10) * ((seq_t)7 * m + (seq_t)13);
        cache[i] = (c1 * cache[i - 1] + c2 * cache[i - 2]) / d;
    }
    filled = n + 1;
    return cache[n];
}
