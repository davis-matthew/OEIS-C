/*
 * A000368  Number of connected graphs with one cycle of length 4.
 * https://oeis.org/A000368
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=2..n-2} S(i)*S(n-i) / Sym^2 (unordered pairs),
 *            where S = Sym^2(T) (coefficients of (T(x)^2 + T(x^2))/2), T(x) =
 *            Sum A000081(k) x^k, i.e. a(n) = [x^n] Sym^2(Sym^2(T)) — the
 *            dihedral (D4) cycle index on a 4-cycle. A000081 via (k-1)*T(k) =
 *            Sum_{j=1..k-1} C(j)*T(k-j), C(j) = Sum_{d|j} d...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 26, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   a368_add, a368_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a368_add.h"
#include "../utils/a368_tri.h"

#define A368_MAXN 20005

static seq_t A368_T[A368_MAXN];  /* A000081 */
static seq_t A368_C[A368_MAXN];  /* sum_{d|k} d*T[d] (low word in fixed mode) */
static seq_t A368_Ch[A368_MAXN]; /* high word of C in fixed mode */
static seq_t A368_S[A368_MAXN];  /* coefficients of Sym^2(T) */
static idx_t A368_have = 0;
static int a368_w = -1;          /* bit width of seq_t, 0 if arbitrary precision */
static int a368_h = 0;
static seq_t a368_M = 0;

static int a368_width(void) {
    if (a368_w < 0) {
        seq_t x = 1;
        int w = 0;
        while (x != 0 && w < 600) { x <<= 1; w++; }
        a368_w = (x == 0) ? w : 0;
        if (a368_w) {
            a368_h = a368_w / 2;
            a368_M = ((seq_t)1 << a368_h) - 1;
        }
    }
    return a368_w;
}

/* full double-word product a*b = (hi, lo) */
static void a368_mul(seq_t a, seq_t b, seq_t *hi, seq_t *lo) {
    int h = a368_h;
    seq_t M = a368_M;
    seq_t a0 = a & M, a1 = a >> h, b0 = b & M, b1 = b >> h;
    seq_t p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    seq_t mid = (p00 >> h) + (p01 & M) + (p10 & M);
    *lo = (p00 & M) | (mid << h);
    *hi = p11 + (p01 >> h) + (p10 >> h) + (mid >> h);
}

/* low word of (hi, lo) / d, d < 2^h */
static seq_t a368_div(seq_t hi, seq_t lo, seq_t d) {
    int h = a368_h;
    seq_t M = a368_M;
    seq_t limbs[4], q[4], r = 0;
    int i;
    limbs[0] = hi >> h; limbs[1] = hi & M; limbs[2] = lo >> h; limbs[3] = lo & M;
    for (i = 0; i < 4; i++) {
        seq_t cur = (r << h) | limbs[i];
        q[i] = cur / d;
        r = cur % d;
    }
    return (q[2] << h) | q[3];
}

static void a368_extend(idx_t m) {
    idx_t k;
    int fixed = a368_width() != 0;
    for (k = A368_have + 1; k <= m; k++) {
        seq_t s, hi, lo, ph, pl;
        idx_t j, d;
        if (k == 1) {
            A368_T[1] = 1;
        } else if (fixed) {
            hi = 0; lo = 0;
            for (j = 1; j < k; j++) {
                a368_mul(A368_C[j], A368_T[k - j], &ph, &pl);
                a368_add(&hi, &lo, ph + A368_Ch[j] * A368_T[k - j], pl);
            }
            A368_T[k] = a368_div(hi, lo, (seq_t)(k - 1));
        } else {
            s = 0;
            for (j = 1; j < k; j++) s += A368_C[j] * A368_T[k - j];
            A368_T[k] = s / (seq_t)(k - 1);
        }
        if (fixed) {
            hi = 0; lo = 0;
            for (d = 1; d * d <= k; d++) {
                if (k % d == 0) {
                    idx_t e = k / d;
                    a368_mul(A368_T[d], (seq_t)d, &ph, &pl);
                    a368_add(&hi, &lo, ph, pl);
                    if (e != d) {
                        a368_mul(A368_T[e], (seq_t)e, &ph, &pl);
                        a368_add(&hi, &lo, ph, pl);
                    }
                }
            }
            A368_C[k] = lo;
            A368_Ch[k] = hi;
        } else {
            s = 0;
            for (d = 1; d * d <= k; d++) {
                if (k % d == 0) {
                    idx_t e = k / d;
                    s += (seq_t)d * A368_T[d];
                    if (e != d) s += (seq_t)e * A368_T[e];
                }
            }
            A368_C[k] = s;
            A368_Ch[k] = 0;
        }
        s = 0;
        for (j = 1; 2 * j < k; j++) s += A368_T[j] * A368_T[k - j];
        if (k % 2 == 0) s += a368_tri(A368_T[k / 2]);
        A368_S[k] = s;
    }
    if (m > A368_have) A368_have = m;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t i;
    int w = a368_width();
    if (n < 4 || n >= A368_MAXN) return 0;
    /* a(n) >= A000081(n-3) >= 2^(n-6): cannot fit beyond this */
    if (w != 0 && n > (idx_t)w + 6) return 0;
    if (A368_have == 0) A368_S[0] = 0;
    a368_extend(n);
    for (i = 2; 2 * i < n; i++) r += A368_S[i] * A368_S[n - i];
    if (n % 2 == 0) r += a368_tri(A368_S[n / 2]);
    return r;
}
