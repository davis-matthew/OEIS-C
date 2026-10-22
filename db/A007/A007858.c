/*
 * A007858  G.f. is 1 - 1/f(x), where f(x) = 1+x+3*x^2+9*x^3+32*x^4+... is 1/x
 *          times g.f. for A063020.
 * https://oeis.org/A007858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   16*(n-1)*n*(2*n-3)*(17*n^2-81*n+96)*a(n) =
 *            (n-1)*(1819*n^4-14124*n^3+40377*n^2-50320*n+23040)*a(n-1) +
 *            8*(2*n-5)*(4*n-11)*(4*n-9)*(17*n^2-47*n+32)*a(n-2), a(1) = 1,
 *            a(2) = 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 32, u128 n <= 60,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"

typedef unsigned __int128 u128;

/* Coefficients of the recurrence; all positive for n >= 3 and below 2^80 for n <= 10^4. */
static void coefs(idx_t n, u128 *A, u128 *B, u128 *D) {
    u128 N = (u128)n;
    u128 N2 = N * N, N3 = N2 * N, N4 = N3 * N;
    u128 p0 = 17 * N2 + 96 - 81 * N;
    u128 p1 = 1819 * N4 + 40377 * N2 + 23040 - 14124 * N3 - 50320 * N;
    u128 p2 = 17 * N2 + 32 - 47 * N;
    *A = (N - 1) * p1;
    *B = 8 * (2 * N - 5) * (4 * N - 11) * (4 * N - 9) * p2;
    *D = 16 * (N - 1) * N * (2 * N - 3) * p0;
}

static u128 *fw = NULL;
static idx_t fw_cap = 0, fw_len = 0;

static seq_t *bg = NULL;
static idx_t bg_cap = 0, bg_len = 0;

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)n;
    if (seq_is_fixed_width()) {
        if (n >= fw_cap) {
            idx_t nc = 2 * n < 16 ? 16 : 2 * n;
            fw = (u128 *)realloc(fw, nc * sizeof(u128));
            fw_cap = nc;
            if (fw_len < 3) { fw[1] = 1; fw[2] = 2; fw_len = 3; }
        }
        while (fw_len <= n) {
            idx_t k = fw_len;
            u128 A, B, D;
            coefs(k, &A, &B, &D);
            u128 a1 = fw[k - 1], a2 = fw[k - 2];
            u128 q1 = a1 / D, r1 = a1 % D;
            u128 q2 = a2 / D, r2 = a2 % D;
            /* A*a1 + B*a2 = D*(A*q1 + B*q2) + (A*r1 + B*r2), exactly divisible */
            fw[k] = A * q1 + B * q2 + (A * r1 + B * r2) / D;
            fw_len++;
        }
        return u128_to_seq(fw[n]);
    } else {
        if (n >= bg_cap) {
            idx_t nc = 2 * n < 16 ? 16 : 2 * n;
            bg = grow_zero(bg, bg_cap, nc);
            bg_cap = nc;
            if (bg_len < 3) { bg[1] = 1; bg[2] = 2; bg_len = 3; }
        }
        while (bg_len <= n) {
            idx_t k = bg_len;
            u128 A, B, D;
            coefs(k, &A, &B, &D);
            seq_t num = u128_to_seq(A) * bg[k - 1] + u128_to_seq(B) * bg[k - 2];
            bg[k] = num / u128_to_seq(D);
            bg_len++;
        }
        return bg[n];
    }
}
