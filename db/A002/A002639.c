/*
 * A002639  Numerators of expansion of Jacobi nome q in parameter m.
 * https://oeis.org/A002639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, frac
 * Formula:   with x = m/16, q = x*Q(x) where F(x) = Sum binomial(2k,k)^2 x^k,
 *            P = F^2, H = 1/((1-16x)*P) (= 1/theta_4(q)^4), Q(0)=1 and n*Q_n
 *            = Sum_{k=1..n} H_k*Q_{n-k} (from x*Q'/Q = H - 1). a(n) =
 *            numerator of Q_{n-1}/16^n, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A2639_NL 12
#define A2639_FMAX 64
#define A2639_BMAX 10003

typedef struct { uint32_t d[A2639_NL]; } a2639_bn;

static void a2639_set(a2639_bn *r, uint32_t v) {
    int i;
    for (i = 0; i < A2639_NL; i++) r->d[i] = 0;
    r->d[0] = v;
}

/* r += x*y (sub=0) or r -= x*y (sub=1), modulo 2^(32*NL) */
static void a2639_addmul(a2639_bn *r, const a2639_bn *x, const a2639_bn *y, int sub) {
    uint32_t t[A2639_NL];
    int i, j;
    uint64_t carry;
    for (i = 0; i < A2639_NL; i++) t[i] = 0;
    for (i = 0; i < A2639_NL; i++) {
        if (x->d[i] == 0) continue;
        carry = 0;
        for (j = 0; i + j < A2639_NL; j++) {
            uint64_t cur = (uint64_t)x->d[i] * y->d[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
    if (!sub) {
        carry = 0;
        for (i = 0; i < A2639_NL; i++) {
            uint64_t cur = (uint64_t)r->d[i] + t[i] + carry;
            r->d[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
    } else {
        uint64_t borrow = 0;
        for (i = 0; i < A2639_NL; i++) {
            uint64_t sub_v = (uint64_t)t[i] + borrow;
            uint64_t cur = (uint64_t)r->d[i];
            if (cur >= sub_v) { r->d[i] = (uint32_t)(cur - sub_v); borrow = 0; }
            else { r->d[i] = (uint32_t)(cur + ((uint64_t)1 << 32) - sub_v); borrow = 1; }
        }
    }
}

static void a2639_mulsm(a2639_bn *r, uint32_t m) {
    uint64_t carry = 0;
    int i;
    for (i = 0; i < A2639_NL; i++) {
        uint64_t cur = (uint64_t)r->d[i] * m + carry;
        r->d[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void a2639_divsm(a2639_bn *r, uint32_t m) {
    uint64_t rem = 0;
    int i;
    for (i = A2639_NL - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | r->d[i];
        r->d[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
}

static int a2639_iszero(const a2639_bn *r) {
    int i;
    for (i = 0; i < A2639_NL; i++) if (r->d[i]) return 0;
    return 1;
}

static void a2639_shr1(a2639_bn *r) {
    int i;
    for (i = 0; i < A2639_NL; i++) {
        uint32_t hi = (i + 1 < A2639_NL) ? r->d[i + 1] : 0;
        r->d[i] = (r->d[i] >> 1) | (hi << 31);
    }
}

static seq_t a2639_tos(const a2639_bn *x) {
    seq_t r = 0;
    int i, b;
    for (i = 3; i >= 0; i--) {
        for (b = 0; b < 32; b++) r += r;
        r += (seq_t)x->d[i];
    }
    return r;
}

/* fixed-width state */
static a2639_bn a2639_fF[A2639_FMAX], a2639_fP[A2639_FMAX],
                a2639_fH[A2639_FMAX], a2639_fQ[A2639_FMAX], a2639_fB;
static idx_t a2639_fdone = 0;

static void a2639_fext(idx_t upto) {
    while (a2639_fdone <= upto) {
        idx_t k = a2639_fdone, j;
        if (k == 0) {
            a2639_set(&a2639_fB, 1);
            a2639_set(&a2639_fF[0], 1);
            a2639_set(&a2639_fP[0], 1);
            a2639_set(&a2639_fH[0], 1);
            a2639_set(&a2639_fQ[0], 1);
        } else {
            a2639_bn acc, neg;
            a2639_mulsm(&a2639_fB, (uint32_t)(2 * (2 * k - 1)));
            a2639_divsm(&a2639_fB, (uint32_t)k);
            a2639_set(&a2639_fF[k], 0);
            a2639_addmul(&a2639_fF[k], &a2639_fB, &a2639_fB, 0);
            a2639_set(&a2639_fP[k], 0);
            for (j = 0; j <= k; j++)
                a2639_addmul(&a2639_fP[k], &a2639_fF[j], &a2639_fF[k - j], 0);
            a2639_set(&acc, 0);
            a2639_set(&neg, 0);
            for (j = 1; j <= k; j++) {
                a2639_addmul(&acc, &a2639_fP[j - 1], &a2639_fH[k - j], 0);
                a2639_addmul(&neg, &a2639_fP[j], &a2639_fH[k - j], 0);
            }
            a2639_mulsm(&acc, 16);
            {
                a2639_bn one;
                a2639_set(&one, 1);
                a2639_addmul(&acc, &neg, &one, 1);
            }
            a2639_fH[k] = acc;
            a2639_set(&acc, 0);
            for (j = 1; j <= k; j++)
                a2639_addmul(&acc, &a2639_fH[j], &a2639_fQ[k - j], 0);
            a2639_divsm(&acc, (uint32_t)k);
            a2639_fQ[k] = acc;
        }
        a2639_fdone++;
    }
}

/* arbitrary-precision state */
static seq_t a2639_bF[A2639_BMAX], a2639_bP[A2639_BMAX],
             a2639_bH[A2639_BMAX], a2639_bQ[A2639_BMAX], a2639_bB;
static idx_t a2639_bdone = 0;

static void a2639_bext(idx_t upto) {
    while (a2639_bdone <= upto) {
        idx_t k = a2639_bdone, j;
        if (k == 0) {
            a2639_bB = 1;
            a2639_bF[0] = 1;
            a2639_bP[0] = 1;
            a2639_bH[0] = 1;
            a2639_bQ[0] = 1;
        } else {
            seq_t pos = 0, neg = 0, s = 0;
            a2639_bB = a2639_bB * ((seq_t)2 * ((seq_t)2 * (seq_t)k - 1)) / (seq_t)k;
            a2639_bF[k] = a2639_bB * a2639_bB;
            for (j = 0; j <= k; j++) s += a2639_bF[j] * a2639_bF[k - j];
            a2639_bP[k] = s;
            for (j = 1; j <= k; j++) {
                pos += a2639_bP[j - 1] * a2639_bH[k - j];
                neg += a2639_bP[j] * a2639_bH[k - j];
            }
            pos *= 16;
            a2639_bH[k] = pos - neg;
            s = 0;
            for (j = 1; j <= k; j++) s += a2639_bH[j] * a2639_bQ[k - j];
            a2639_bQ[k] = s / (seq_t)k;
        }
        a2639_bdone++;
    }
}

static int a2639_mode = -1; /* 1 = fixed width, 0 = big */

seq_t a(idx_t n) {
    if (a2639_mode < 0) {
        seq_t x = 1;
        int w = 0;
        while (w < 300 && x != 0) { x += x; w++; }
        a2639_mode = (x == 0) ? 1 : 0;
    }
    if (n == 0) return 0;
    if (a2639_mode == 1) {
        a2639_bn c;
        idx_t cnt = 0;
        if (n - 1 >= A2639_FMAX) return 0; /* cannot fit in <=128 bits */
        a2639_fext(n - 1);
        c = a2639_fQ[n - 1];
        while (cnt < 4 * n && !a2639_iszero(&c) && (c.d[0] & 1) == 0) {
            a2639_shr1(&c);
            cnt++;
        }
        return a2639_tos(&c);
    } else {
        seq_t c;
        idx_t cnt = 0;
        if (n - 1 >= A2639_BMAX) return 0;
        a2639_bext(n - 1);
        c = a2639_bQ[n - 1];
        while (cnt < 4 * n && c != 0 && c % 2 == 0) {
            c /= 2;
            cnt++;
        }
        return c;
    }
}
