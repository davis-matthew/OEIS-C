/*
 * A005695  Positions of remoteness 3 in Beans-Don't-Talk.
 * https://oeis.org/A005695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Beans-Don't-Talk moves n -> odd part
 *            of 3n+1 or of 3n-1; 1 is terminal (remoteness 0). R1 =
 *            {(2^k+-1)/3 : k>=3} (Jacobsthal numbers >= 3) = positions with a
 *            move to 1. R2 = {m >= 2 : odd(3m+1), odd(3m-1) both in R1}
 *            (P-positions of remoteness 2). a(n) = n-th smallest x...
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"

typedef unsigned __int128 u128_t;

#define U_ALLOC(n) ((u128_t *)calloc((n), sizeof(u128_t)))
#define U_FREE(p, n) free(p)
#define S_ALLOC(n) alloc_zero((idx_t)(n))
#define S_FREE(p, n) release((p), (idx_t)(n))

#define DEF_GEN(T, S, ALLOC, FREE) \
static int isR1_##S(T v) { \
    T x, y; \
    if (v < 3 || (v & 1) == 0) return 0; \
    x = v * 3 + 1; y = v * 3 - 1; \
    return ((x & (x - 1)) == 0) || ((y & (y - 1)) == 0); \
} \
static T odd_##S(T x) { while ((x & 1) == 0) x >>= 1; return x; } \
static void sift_##S(T *a, size_t i, size_t n) { \
    T t = a[i]; \
    for (;;) { \
        size_t c = 2 * i + 1; \
        if (c >= n) break; \
        if (c + 1 < n && a[c] < a[c + 1]) c++; \
        if (!(t < a[c])) break; \
        a[i] = a[c]; i = c; \
    } \
    a[i] = t; \
} \
static size_t sortuniq_##S(T *a, size_t n) { \
    size_t i, end, k = 0; T t; \
    if (n < 2) return n; \
    for (i = n / 2; i-- > 0;) sift_##S(a, i, n); \
    for (end = n - 1; end > 0; end--) { \
        t = a[0]; a[0] = a[end]; a[end] = t; sift_##S(a, 0, end); \
    } \
    for (i = 0; i < n; i++) \
        if (k == 0 || !(a[i] == a[k - 1])) a[k++] = a[i]; \
    return k; \
} \
static T back_##S(T x) { /* m with 3m+1 = x or 3m-1 = x, else 0 */ \
    T r = x % 3; \
    if (r == 1) return (x - 1) / 3; \
    if (r == 2) return (x + 1) / 3; \
    return (T)0; \
} \
static size_t gen_##S(int L, T **out, size_t *outcap) { \
    T B = (T)1 << L; \
    T M2 = B * 3 + 1, M1 = M2 * 3 + 1, x, m; \
    size_t c1cap = (size_t)L + 12, c1 = 0, c2cap, c2 = 0, c3cap, c3 = 0, i; \
    T *r1 = ALLOC(c1cap), *r2, *r3; \
    int k; \
    for (k = 3;; k++) { \
        T p = (T)1 << k, j; \
        if (k & 1) j = (p + 1) / 3; else j = (p - 1) / 3; \
        if (j > M1) break; \
        r1[c1++] = j; \
    } \
    c2cap = c1 * ((size_t)L + 12) + 1; \
    r2 = ALLOC(c2cap); \
    for (i = 0; i < c1; i++) { \
        for (x = r1[i]; x <= M1; x <<= 1) { \
            m = back_##S(x); \
            if (m >= 2 && m <= M2 && isR1_##S(odd_##S(m * 3 + 1)) \
                && isR1_##S(odd_##S(m * 3 - 1))) \
                r2[c2++] = m; \
        } \
    } \
    c2 = sortuniq_##S(r2, c2); \
    c3cap = c2 * ((size_t)L + 8) + 1; \
    r3 = ALLOC(c3cap); \
    for (i = 0; i < c2; i++) { \
        for (x = r2[i]; x <= M2; x <<= 1) { \
            m = back_##S(x); \
            if (m >= 2 && m <= B && !isR1_##S(m)) r3[c3++] = m; \
        } \
    } \
    c3 = sortuniq_##S(r3, c3); \
    FREE(r1, c1cap); \
    FREE(r2, c2cap); \
    *out = r3; *outcap = c3cap; \
    return c3; \
}

DEF_GEN(u128_t, u, U_ALLOC, U_FREE)
DEF_GEN(seq_t, s, S_ALLOC, S_FREE)

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) {
        static u128_t *cu = NULL;
        static size_t ncu = 0, capu = 0;
        static int Lu = 0;
        int L = 16;
        for (;;) {
            if (L > 120) L = 120;
            if (L > Lu) {
                if (cu) U_FREE(cu, capu);
                ncu = gen_u(L, &cu, &capu);
                Lu = L;
            }
            if ((idx_t)ncu >= n) return u128_to_seq(cu[n - 1]);
            if (L >= 120) return (seq_t)0;
            L += L / 2 + 8;
        }
    } else {
        static seq_t *cs = NULL;
        static size_t ncs = 0, caps = 0;
        static int Ls = 0;
        int L = 16;
        for (;;) {
            if (L > Ls) {
                if (cs) S_FREE(cs, caps);
                ncs = gen_s(L, &cs, &caps);
                Ls = L;
            }
            if ((idx_t)ncs >= n) return cs[n - 1];
            L += L / 2 + 8;
        }
    }
}
