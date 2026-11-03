/*
 * A005913  a(n) = floor(tau*a(n-1)) + floor(tau*a(n-2)) with a(0)=1 and
 *          a(1)=3.
 * https://oeis.org/A005913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(tau*a(n-1)) + floor(tau*a(n-2)), a(0) = 1, a(1) =
 *            3, tau = (1+sqrt(5))/2; floor(tau*x) = floor((x +
 *            isqrt(5*x^2))/2), or via the Zeckendorf shift for fixed-width
 *            types
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 978 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

/* floor(tau*x) using Zeckendorf representation: x = sum F_k  =>
   sum F_{k+1} = tau*x + sum psi^k, sign of the error given by parity of smallest k */
static seq_t floor_phi(seq_t x) {
    seq_t f[200];
    int m, k, kmin = 0;
    seq_t r, S;
    if (x == 0) return 0;
    f[1] = 1;
    f[2] = 1;
    m = 2;
    while (m < 199) {
        seq_t nx = f[m] + f[m - 1];
        if (nx < f[m] || nx > x) break;
        f[++m] = nx;
    }
    r = x;
    S = 0;
    for (k = m; k >= 2; k--) {
        if (f[k] <= r) {
            r -= f[k];
            S += f[k];
            S += f[k - 1];
            kmin = k;
            k--;
        }
    }
    if (kmin % 2 == 0) S -= 1;
    return S;
}

static seq_t *A = NULL, *T = NULL;
static idx_t cap = 0, len = 0;

seq_t a(idx_t n) {
    bool fixed;
    if (n + 1 >= cap) {
        idx_t nc = 2 * (n + 1);
        if (nc < 16) nc = 16;
        if (!A) {
            A = alloc_zero(nc);
            T = alloc_zero(nc);
        } else {
            A = grow_zero(A, cap, nc);
            T = grow_zero(T, cap, nc);
        }
        cap = nc;
    }
    if (len == 0) {
        A[0] = 1; A[1] = 3;
        T[0] = 2; T[1] = 6;   /* isqrt(5*a^2) */
        len = 2;
    }
    fixed = seq_is_fixed_width();
    while (len <= n) {
        idx_t i = len;
        if (fixed) {
            A[i] = floor_phi(A[i - 1]) + floor_phi(A[i - 2]);
        } else {
            seq_t x1 = A[i - 1], x2 = A[i - 2];
            seq_t c1 = (x1 + T[i - 1]) / 2;
            seq_t c2 = (x2 + T[i - 2]) / 2;
            seq_t x = c1 + c2;
            seq_t g, N, G;
            A[i] = x;
            /* upper estimate of sqrt(5)*x, within a few units */
            g = (T[i - 1] + 5 * x1) / 2 + (T[i - 2] + 5 * x2) / 2 + 3;
            N = 5 * x * x;
            G = g * g;
            while (G > N) {
                G -= 2 * g;
                G += 1;
                g -= 1;
            }
            T[i] = g;
        }
        len++;
    }
    return A[n];
}
