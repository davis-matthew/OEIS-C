/*
 * A004790  Numbers k >= 2 such that if 1 < j < k then (fractional part of log
 *          k) < (fractional part of log j).
 * https://oeis.org/A004790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(1) = 2; for m = 1, 2, ..., k = ceiling(e^m) is a term
 *            exactly when frac(log k) = log(k) - m is a new record low
 *            (counting frac(log 2) as the first record). e^m is evaluated in
 *            exact multi-limb fixed-point arithmetic with enough guard bits
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   a4790_frac_limbs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/a4790_frac_limbs.h"

typedef unsigned __int128 a4790_u128;

/* fixed-point multiply: A, B have F fractional 64-bit limbs; result has F fractional limbs */
static uint64_t *a4790_mul(const uint64_t *A, size_t na, const uint64_t *B, size_t nb,
                           size_t F, size_t *nout)
{
    size_t lo = F - 2;
    size_t rn = na + nb - lo;
    uint64_t *r = (uint64_t *)calloc(rn + 1, sizeof(uint64_t));
    size_t i, j, n;
    for (i = 0; i < na; i++) {
        uint64_t ai = A[i];
        a4790_u128 carry = 0;
        size_t j0, idx;
        if (ai == 0) continue;
        j0 = (i >= lo) ? 0 : lo - i;
        for (j = j0; j < nb; j++) {
            a4790_u128 t;
            idx = i + j - lo;
            t = (a4790_u128)ai * B[j] + r[idx] + carry;
            r[idx] = (uint64_t)t;
            carry = t >> 64;
        }
        idx = i + nb - lo;
        while (carry) {
            a4790_u128 t = (a4790_u128)r[idx] + carry;
            r[idx] = (uint64_t)t;
            carry = t >> 64;
            idx++;
        }
    }
    n = rn - 2;
    memmove(r, r + 2, n * sizeof(uint64_t));
    while (n > F + 1 && r[n - 1] == 0) n--;
    *nout = n;
    return r;
}

/* e with F fractional limbs (F+1 limbs total) */
static uint64_t *a4790_e(size_t F)
{
    size_t G = F + 1, n = G + 1, top = G, i;
    uint64_t *s = (uint64_t *)calloc(n, sizeof(uint64_t));
    uint64_t *t = (uint64_t *)calloc(n, sizeof(uint64_t));
    uint64_t *r = (uint64_t *)malloc((F + 1) * sizeof(uint64_t));
    uint64_t k;
    s[G] = 1;
    t[G] = 1;
    for (k = 1;; k++) {
        a4790_u128 rem = 0, c = 0;
        for (i = top + 1; i-- > 0;) {
            a4790_u128 cur = (rem << 64) | t[i];
            t[i] = (uint64_t)(cur / k);
            rem = cur % k;
        }
        while (top > 0 && t[top] == 0) top--;
        if (top == 0 && t[0] == 0) break;
        for (i = 0; i < n; i++) {
            c += (a4790_u128)s[i] + t[i];
            s[i] = (uint64_t)c;
            c >>= 64;
        }
    }
    for (i = 0; i <= F; i++) r[i] = s[i + 1];
    free(s);
    free(t);
    return r;
}

static idx_t *a4790_rec = NULL;
static size_t a4790_cnt = 0, a4790_cap = 0;
static idx_t a4790_scanned = 0;

static void a4790_scan(idx_t M)
{
    size_t F = a4790_frac_limbs(M);
    uint64_t *E = a4790_e(F);
    uint64_t *X = (uint64_t *)malloc((F + 1) * sizeof(uint64_t));
    size_t nx = F + 1;
    double best = log(log(2.0));
    idx_t m;
    memcpy(X, E, (F + 1) * sizeof(uint64_t));
    a4790_cnt = 0;
    for (m = 1; m <= M; m++) {
        size_t p = F, q;
        uint64_t hi = 1, lo = 0;
        double ld, lf;
        if (m > 1) {
            size_t nn;
            uint64_t *Y = a4790_mul(X, nx, E, F + 1, F, &nn);
            free(X);
            X = Y;
            nx = nn;
        }
        /* delta = ceiling(e^m) - e^m = 1 - frac(X) */
        for (q = F; q-- > 0;) {
            if (~X[q] != 0) {
                p = q;
                hi = ~X[q];
                lo = q > 0 ? ~X[q - 1] : 0;
                break;
            }
        }
        if (p == F) p = 0;
        ld = log((double)hi + (double)lo / 18446744073709551616.0)
             - 64.0 * (double)(F - p) * 0.69314718055994530942;
        if (m <= 200)
            lf = log(log1p(exp(ld) / exp((double)m)));
        else
            lf = ld - (double)m;
        if (lf < best) {
            best = lf;
            if (a4790_cnt == a4790_cap) {
                a4790_cap = a4790_cap ? 2 * a4790_cap : 256;
                a4790_rec = (idx_t *)realloc(a4790_rec, a4790_cap * sizeof(idx_t));
            }
            a4790_rec[a4790_cnt++] = m;
        }
    }
    free(X);
    free(E);
    a4790_scanned = M;
}

static seq_t a4790_ceil_exp(idx_t m)
{
    size_t F = a4790_frac_limbs(m);
    uint64_t *B = a4790_e(F);
    size_t nb = F + 1, nr = F + 1, i;
    uint64_t *R = (uint64_t *)calloc(F + 1, sizeof(uint64_t));
    idx_t p = m;
    seq_t res = 0;
    a4790_u128 c = 1;
    R[F] = 1;
    while (p) {
        size_t nn;
        if (p & 1) {
            uint64_t *T = a4790_mul(R, nr, B, nb, F, &nn);
            free(R);
            R = T;
            nr = nn;
        }
        p >>= 1;
        if (p) {
            uint64_t *T = a4790_mul(B, nb, B, nb, F, &nn);
            free(B);
            B = T;
            nb = nn;
        }
    }
    /* integer part + 1 */
    R = (uint64_t *)realloc(R, (nr + 1) * sizeof(uint64_t));
    R[nr] = 0;
    for (i = F; i <= nr && c; i++) {
        c += R[i];
        R[i] = (uint64_t)c;
        c >>= 64;
    }
    if (R[nr] != 0) nr++;
    for (i = nr; i-- > F;) {
        int b;
        for (b = 56; b >= 0; b -= 8) {
            unsigned byte = (unsigned)((R[i] >> b) & 0xFF);
            res = res * 256 + (seq_t)byte;
        }
    }
    free(R);
    free(B);
    return res;
}

seq_t a(idx_t n)
{
    size_t need;
    if (n <= 1) return 2;
    need = (size_t)(n - 1);
    if (a4790_cnt < need) {
        idx_t M = (idx_t)need * 3 / 2 + 64;
        if (a4790_scanned * 2 > M) M = a4790_scanned * 2;
        for (;;) {
            a4790_scan(M);
            if (a4790_cnt >= need) break;
            M *= 2;
        }
    }
    return a4790_ceil_exp(a4790_rec[need - 1]);
}
