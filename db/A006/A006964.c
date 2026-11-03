/*
 * A006964  Number of directed rooted trees with n nodes.
 * https://oeis.org/A006964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(1) = 1; a(m+1) = (1/m) * Sum_{k=1..m} b(k)*a(m+1-k), where
 *            b(k) = 3*Sum_{d|k} d*a(d) (from A(x) = x*exp(3*Sum_{k>=1}
 *            A(x^k)/k))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 496 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

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

/* Fixed width: add x*y*z to Q*m + R, never forming anything larger than the quotient.
   Returns 1 on overflow. */
static int acc_term(seq_t *Q, uint64_t *R, uint64_t m, uint64_t x, seq_t y, seq_t z, seq_t M)
{
    seq_t yq = y / m;
    uint64_t yr = (uint64_t)(long long)(y % m);
    uint64_t xy = x * yr;
    seq_t pq, t, zq;
    uint64_t pr, zr, q2;

    if (yq != 0 && yq > M / x) return 1;
    pq = yq * x;
    q2 = xy / m;
    if (pq > M - q2) return 1;
    pq += q2;
    pr = xy % m;

    if (pq != 0 && z > M / pq) return 1;
    t = pq * z;
    if (*Q > M - t) return 1;
    *Q += t;

    zq = z / m;
    zr = (uint64_t)(long long)(z % m);
    if (pr != 0) {
        if (zq > M / pr) return 1;
        t = zq * pr;
        if (*Q > M - t) return 1;
        *Q += t;
    }
    *R += pr * zr;
    if (*R >= m) {
        q2 = *R / m;
        if (*Q > M - q2) return 1;
        *Q += q2;
        *R %= m;
    }
    return 0;
}

static seq_t *A_ = NULL;   /* A_[1..have] */
static seq_t *B_ = NULL;   /* B_[k] = b(k), used for the arbitrary-precision build */
static idx_t have = 0;
static idx_t cap = 0;
static idx_t limit = 0;    /* fixed width: first index whose value overflowed (0 = none yet) */

static void ensure_cap(idx_t need)
{
    if (need < cap) return;
    idx_t nc = cap ? cap : 64;
    while (nc <= need) nc *= 2;
    A_ = grow_zero(A_, cap, nc);
    B_ = grow_zero(B_, cap, nc);
    cap = nc;
}

seq_t a(idx_t n)
{
    bool fixed = seq_is_fixed_width();

    if (n < 1) return 0;
    if (have == 0) {
        ensure_cap(2);
        A_[1] = 1;
        have = 1;
    }
    if (fixed && limit != 0 && n >= limit) return 0;
    if (n <= have) return A_[n];

    ensure_cap(n + 1);

    while (have < n) {
        idx_t m = have;  /* compute a(m+1) */
        if (!fixed) {
            seq_t bm = 0, S = 0;
            idx_t d, k;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    bm += A_[d] * d;
                    if (e != d) bm += A_[e] * e;
                }
            }
            B_[m] = bm * 3;
            for (k = 1; k <= m; k++)
                S += B_[k] * A_[m + 1 - k];
            A_[m + 1] = S / m;
        } else {
            seq_t M = 0;
            seq_t Q = 0;
            uint64_t R = 0;
            int ovf = 0;
            idx_t d, k;
            M--;
            /* Sum over k = d*j, d | k: 3*d*a(d)*a(m+1-k) */
            for (d = 1; d <= m && !ovf; d++) {
                for (k = d; k <= m; k += d) {
                    if (acc_term(&Q, &R, (uint64_t)m, (uint64_t)3 * d, A_[d], A_[m + 1 - k], M)) {
                        ovf = 1;
                        break;
                    }
                }
            }
            if (ovf) {
                limit = m + 1;
                if (n >= limit) return 0;
                break;
            }
            A_[m + 1] = Q;
        }
        have = m + 1;
    }
    return A_[n];
}
