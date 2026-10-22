/*
 * A001171  From least significant term in expansion of E( tr (X'*X)^n ), X
 *          rectangular and Gaussian. Also number of types of sequential
 *          n-swap moves for traveling salesman problem.
 * https://oeis.org/A001171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   (n+1)*a(n) = -(4*n-1)*a(n-1) + (5*n^3-16*n^2+13*n-1)*a(n-2) +
 *            (10*n^3-68*n^2+150*n-107)*a(n-3) -
 *            (n-4)*(n-2)^2*(2*n-7)^2*a(n-4) for n >= 5, a(1) = 1, a(2) = 1,
 *            a(3) = 4, a(4) = 20. Fixed-width builds run the recurrence in an
 *            exact internal multi-limb integer (the division by n+1 is not
 *            valid modu...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   b1171_big, b1171_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/b1171_big.h"
#include "../utils/b1171_is_fixed.h"

#define B1171_L 48
#define B1171_CAP 80

static void b1171_muls(uint32_t *x, uint32_t m) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < B1171_L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void b1171_add(uint32_t *x, const uint32_t *y) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < B1171_L; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void b1171_sub(uint32_t *x, const uint32_t *y) {
    int64_t br = 0;
    int i;
    for (i = 0; i < B1171_L; i++) {
        int64_t t = (int64_t)x[i] - (int64_t)y[i] - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        x[i] = (uint32_t)t;
    }
}

static void b1171_divs(uint32_t *x, uint32_t d) {
    uint64_t r = 0;
    int i;
    for (i = B1171_L - 1; i >= 0; i--) {
        uint64_t t = (r << 32) | x[i];
        x[i] = (uint32_t)(t / d);
        r = t % d;
    }
}

static seq_t b1171_fixed(idx_t n) {
    uint32_t v[4][B1171_L];
    uint32_t P[B1171_L], Q[B1171_L], T[B1171_L];
    seq_t r = (seq_t)0;
    idx_t k;
    int i, b;
    if (n > B1171_CAP) return (seq_t)0;
    memset(v, 0, sizeof(v));
    v[1][0] = 1; v[2][0] = 1; v[3][0] = 4; v[0][0] = 20;
    for (k = 5; k <= n; k++) {
        uint32_t kk = (uint32_t)k;
        uint32_t c1 = 4 * kk - 1;
        uint32_t c2 = 5 * kk * kk * kk + 13 * kk - 16 * kk * kk - 1;
        uint32_t c3 = 10 * kk * kk * kk + 150 * kk - 68 * kk * kk - 107;
        memcpy(P, v[(k - 2) & 3], sizeof(P));
        b1171_muls(P, c2);
        memcpy(T, v[(k - 3) & 3], sizeof(T));
        b1171_muls(T, c3);
        b1171_add(P, T);
        memcpy(Q, v[(k - 1) & 3], sizeof(Q));
        b1171_muls(Q, c1);
        memcpy(T, v[(k - 4) & 3], sizeof(T));
        b1171_muls(T, kk - 4);
        b1171_muls(T, kk - 2);
        b1171_muls(T, kk - 2);
        b1171_muls(T, 2 * kk - 7);
        b1171_muls(T, 2 * kk - 7);
        b1171_add(Q, T);
        b1171_sub(P, Q);
        b1171_divs(P, kk + 1);
        memcpy(v[k & 3], P, sizeof(P));
    }
    for (i = B1171_L - 1; i >= 0; i--) {
        for (b = 3; b >= 0; b--) {
            r = r * (seq_t)256 + (seq_t)((v[n & 3][i] >> (8 * b)) & 255u);
        }
    }
    return r;
}

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)20;
    if (b1171_is_fixed()) return b1171_fixed(n);
    return b1171_big(n);
}
