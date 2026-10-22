/*
 * A000243  Number of trees with n nodes, 2 of which are labeled.
 * https://oeis.org/A000243
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. A(x) = B(x)^2/(1-B(x)) = B(x)*C(x) with C(x) =
 *            B(x)/(1-B(x)), i.e. C = B + B*C, where B(x) = Sum b(n) x^n is
 *            the g.f. of A000081: b(1) = 1, m*b(m+1) = Sum_{k=1..m}
 *            (Sum_{d|k} d*b(d)) * b(m-k+1); a(n) = Sum_{i=1..n-1}
 *            b(i)*c(n-i), c(1) = 1, c(m) = b(m) + Sum_{i=1..m-1} b(i)*c(m-i)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

#define A243_MAXN 20005

#define A243_IMPL(T, SFX) \
static T a243_b_##SFX[A243_MAXN]; \
static T a243_s_##SFX[A243_MAXN]; \
static T a243_c_##SFX[A243_MAXN]; \
static idx_t a243_done_##SFX = 0; \
static T a243_calc_##SFX(idx_t n) \
{ \
    T *B = a243_b_##SFX, *S = a243_s_##SFX, *C = a243_c_##SFX; \
    T r = 0; \
    idx_t i; \
    while (a243_done_##SFX < n) { \
        idx_t k = a243_done_##SFX + 1, d; \
        T v = 1, s = 0, c; \
        if (k > 1) { \
            T t = 0; \
            idx_t j = k - 1; \
            for (i = 1; i <= j; i++) t += S[i] * B[j - i + 1]; \
            v = t / (T)j; \
        } \
        B[k] = v; \
        for (d = 1; d * d <= k; d++) \
            if (k % d == 0) { \
                s += (T)d * B[d]; \
                if (k / d != d) s += (T)(k / d) * B[k / d]; \
            } \
        S[k] = s; \
        c = v; \
        for (i = 1; i < k; i++) c += B[i] * C[k - i]; \
        C[k] = c; \
        a243_done_##SFX = k; \
    } \
    for (i = 1; i < n; i++) r += B[i] * C[n - i]; \
    return r; \
}

/* 128-bit internal path for fixed-width builds (keeps m*b(m+1) exact
   whenever the terms themselves fit in 64 bits), and a seq_t path for
   arbitrary precision. */
A243_IMPL(unsigned __int128, w)
A243_IMPL(seq_t, s)

seq_t a(idx_t n)
{
    if (n < 2 || n >= A243_MAXN) return 0;
    if (seq_is_fixed_width()) return u128_to_seq(a243_calc_w(n));
    return a243_calc_s(n);
}
