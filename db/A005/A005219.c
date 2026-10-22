/*
 * A005219  Number of unlabeled identity unit interval graphs.
 * https://oeis.org/A005219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = [x^n] Product_{k>=2} (1+x^k)^c(k), where c(k) = (R(k-1) -
 *            s(k))/2 counts the connected identity unit interval graphs, R =
 *            Riordan numbers A005043 (R(0)=1, R(1)=0, (m+1)R(m) =
 *            (m-1)(2R(m-1)+3R(m-2))), s(2m+1) = T(m), s(2m) =
 *            (T(m)-T(m-1))/2, T = central trinomial coefficients A002426
 *            (T(0)=...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 48, u128 n <= 88,
 *            big any n
 * Helpers:   a5219_gsm, alloc_zero, release, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a5219_gsm.h"

/* c[0..N] computed in unsigned __int128 (fixed-width builds) */
static void a5219_c_u128(seq_t *c, idx_t N) {
    unsigned __int128 *R = (unsigned __int128 *)malloc((size_t)(N + 2) * sizeof(unsigned __int128));
    unsigned __int128 *T = (unsigned __int128 *)malloc((size_t)(N / 2 + 3) * sizeof(unsigned __int128));
    idx_t i, m;
    R[0] = 1; R[1] = 0;
    for (i = 2; i <= N; i++) {
        unsigned __int128 X = 2 * R[i - 1] + 3 * R[i - 2];
        idx_t g = (i & 1) ? 2 : 1;
        R[i] = X / ((i + 1) / g) * ((i - 1) / g);
    }
    T[0] = 1; T[1] = 1;
    for (m = 2; m <= N / 2 + 1; m++)
        T[m] = ((unsigned __int128)(2 * m - 1) * T[m - 1] + (unsigned __int128)(3 * (m - 1)) * T[m - 2]) / m;
    c[0] = 0;
    for (i = 1; i <= N; i++) {
        unsigned __int128 t = R[i - 1], s;
        if (i & 1) s = T[(i - 1) / 2];
        else s = (T[i / 2] - T[i / 2 - 1]) / 2;
        c[i] = u128_to_seq((t >> 1) - (s >> 1));
    }
    free(R); free(T);
}

/* c[0..N] computed exactly in seq_t (arbitrary-precision builds) */
static void a5219_c_seq(seq_t *c, idx_t N) {
    seq_t *R = alloc_zero(N + 2);
    seq_t *T = alloc_zero(N / 2 + 3);
    idx_t i, m;
    R[0] = 1; R[1] = 0;
    for (i = 2; i <= N; i++) {
        seq_t X = R[i - 1] * 2 + R[i - 2] * 3;
        idx_t g = (i & 1) ? 2 : 1;
        R[i] = X / ((i + 1) / g) * ((i - 1) / g);
    }
    T[0] = 1; T[1] = 1;
    for (m = 2; m <= N / 2 + 1; m++)
        T[m] = (T[m - 1] * (2 * m - 1) + T[m - 2] * (3 * (m - 1))) / m;
    c[0] = 0;
    for (i = 1; i <= N; i++) {
        seq_t t = R[i - 1], s;
        if (i & 1) s = T[(i - 1) / 2];
        else s = (T[i / 2] - T[i / 2 - 1]) / 2;
        c[i] = (t >> 1) - (s >> 1);
    }
    release(R, N + 2);
    release(T, N / 2 + 3);
}

static seq_t *a5219_A = NULL;
static idx_t a5219_N = 0;

seq_t a(idx_t n) {
    bool fixed = seq_is_fixed_width();
    idx_t bound = (idx_t)(8 * sizeof(seq_t)) + 20;
    if (fixed && n > bound) return 0; /* a(n) > 2^n here: cannot fit */
    if (a5219_A == NULL || n > a5219_N) {
        idx_t N, k, j, m;
        seq_t *c, *P, *bin;
        if (fixed) N = bound;
        else {
            N = n;
            if (N < 2 * a5219_N) N = 2 * a5219_N;
            if (N < 64) N = 64;
        }
        c = alloc_zero(N + 1);
        if (fixed) a5219_c_u128(c, N);
        else a5219_c_seq(c, N);
        P = alloc_zero(N + 1);
        P[0] = 1;
        bin = alloc_zero(N / 2 + 2);
        for (k = 2; k <= N; k++) {
            idx_t J;
            if (c[k] == 0) continue;
            J = N / k;
            bin[0] = 1;
            for (j = 1; j <= J; j++) {
                seq_t B = bin[j - 1];
                uint64_t g;
                if (c[k] < (seq_t)j) { J = j - 1; break; }
                g = a5219_gsm((uint64_t)(B % j), (uint64_t)j);
                bin[j] = (B / g) * ((c[k] - (j - 1)) / (j / g));
            }
            for (m = N; m >= k; m--) {
                seq_t acc = P[m];
                for (j = 1; j <= J && j * k <= m; j++)
                    acc += bin[j] * P[m - j * k];
                P[m] = acc;
            }
        }
        P[1] = 0;
        release(c, N + 1);
        release(bin, N / 2 + 2);
        if (a5219_A) release(a5219_A, a5219_N + 1);
        a5219_A = P;
        a5219_N = N;
    }
    return a5219_A[n];
}
