/*
 * A074959  Decimal expansion of the arithmetic-geometric mean (AGM) of 3 and
 *          5.
 * https://oeis.org/A074959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n-1) * AGM(3,5)) mod 10, where AGM(a0,b0) is
 *            the limit of a_{k+1}=(a_k+b_k)/2, b_{k+1}=sqrt(a_k*b_k), a_0=3,
 *            b_0=5; computed with fixed-point big-integer arithmetic
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t u32_;
typedef uint64_t u64_;

static void bn_mul(const u32_ *x, const u32_ *y, int L, u32_ *out) {
    int i, j;
    for (i = 0; i < 2 * L; i++) out[i] = 0;
    for (i = 0; i < L; i++) {
        u64_ carry = 0;
        u64_ xi = x[i];
        if (xi == 0) continue;
        for (j = 0; j < L; j++) {
            u64_ t = xi * y[j] + out[i + j] + carry;
            out[i + j] = (u32_)t;
            carry = t >> 32;
        }
        out[i + L] = (u32_)carry;
    }
}

static void bn_shl(u32_ *x, int M, int k) { /* k in 1..31 */
    int i;
    for (i = M - 1; i > 0; i--)
        x[i] = (x[i] << k) | (x[i - 1] >> (32 - k));
    x[0] <<= k;
}

static int bn_cmp(const u32_ *x, const u32_ *y, int M) {
    int i;
    for (i = M - 1; i >= 0; i--) {
        if (x[i] != y[i]) return x[i] > y[i] ? 1 : -1;
    }
    return 0;
}

static void bn_sub(u32_ *x, const u32_ *y, int M) { /* x -= y, x>=y */
    int i;
    u64_ borrow = 0;
    for (i = 0; i < M; i++) {
        u64_ t = (u64_)x[i] - y[i] - borrow;
        x[i] = (u32_)t;
        borrow = (t >> 63) & 1;
    }
}

/* A has 2L limbs; root gets L limbs = floor(sqrt(A)) */
static void bn_sqrt(const u32_ *A, int L, u32_ *root) {
    int M = L + 2, j, i;
    u32_ *rem = (u32_ *)calloc(M, sizeof(u32_));
    u32_ *trial = (u32_ *)calloc(M, sizeof(u32_));
    for (i = 0; i < L; i++) root[i] = 0;
    for (j = 32 * L - 1; j >= 0; j--) {
        u32_ v = (A[(2 * j) / 32] >> ((2 * j) % 32)) & 3u;
        bn_shl(rem, M, 2);
        rem[0] |= v;
        for (i = 0; i < M; i++) trial[i] = (i < L) ? root[i] : 0;
        bn_shl(trial, M, 2);
        trial[0] |= 1;
        {
            int bit = 0;
            if (bn_cmp(rem, trial, M) >= 0) {
                bn_sub(rem, trial, M);
                bit = 1;
            }
            {
                int k;
                for (k = L - 1; k > 0; k--)
                    root[k] = (root[k] << 1) | (root[k - 1] >> 31);
                root[0] = (root[0] << 1) | (u32_)bit;
            }
        }
    }
    free(rem);
    free(trial);
}

static unsigned char *g_dig = 0;
static long long g_cap = 0;

static void compute_digits(long long cap) {
    long long D = cap + 25;
    int F = (int)((D * 3322LL) / 1000 / 32) + 3;
    int L = F + 1, i, iter;
    u32_ *a = (u32_ *)calloc(L, sizeof(u32_));
    u32_ *b = (u32_ *)calloc(L, sizeof(u32_));
    u32_ *na = (u32_ *)calloc(L, sizeof(u32_));
    u32_ *nb = (u32_ *)calloc(L, sizeof(u32_));
    u32_ *prod = (u32_ *)calloc(2 * L, sizeof(u32_));
    u32_ *d = (u32_ *)calloc(L, sizeof(u32_));
    a[F] = 3;
    b[F] = 5;
    for (iter = 0; iter < 200; iter++) {
        u64_ carry = 0;
        /* na = (a+b)/2 */
        for (i = 0; i < L; i++) {
            u64_ t = (u64_)a[i] + b[i] + carry;
            na[i] = (u32_)t;
            carry = t >> 32;
        }
        for (i = 0; i < L; i++) {
            u32_ hi = (i + 1 < L) ? na[i + 1] : (u32_)carry;
            na[i] = (na[i] >> 1) | (hi << 31);
        }
        bn_mul(a, b, L, prod);
        bn_sqrt(prod, L, nb);
        memcpy(a, na, L * sizeof(u32_));
        memcpy(b, nb, L * sizeof(u32_));
        /* diff */
        if (bn_cmp(a, b, L) >= 0) {
            memcpy(d, a, L * sizeof(u32_));
            bn_sub(d, b, L);
        } else {
            memcpy(d, b, L * sizeof(u32_));
            bn_sub(d, a, L);
        }
        {
            int small = 1;
            for (i = 1; i < L; i++) if (d[i]) { small = 0; break; }
            if (small && d[0] <= 8) break;
        }
    }
    free(g_dig);
    g_dig = (unsigned char *)malloc((size_t)cap + 1);
    g_cap = cap;
    g_dig[1] = (unsigned char)a[F];
    {
        long long k;
        for (k = 2; k <= cap; k++) {
            u64_ carry = 0;
            for (i = 0; i < F; i++) {
                u64_ t = (u64_)a[i] * 10 + carry;
                a[i] = (u32_)t;
                carry = t >> 32;
            }
            g_dig[k] = (unsigned char)carry;
        }
    }
    free(a); free(b); free(na); free(nb); free(prod); free(d);
}

seq_t a(idx_t n) {
    long long nn = (long long)n;
    if (nn > g_cap || !g_dig) {
        long long cap = nn * 2;
        if (cap < 256) cap = 256;
        compute_digits(cap);
    }
    return (seq_t)g_dig[nn];
}
