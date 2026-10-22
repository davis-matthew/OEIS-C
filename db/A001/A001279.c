/*
 * A001279  Number of permutations of length n by rises.
 * https://oeis.org/A001279
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   3*(n-6)*a(n) = (3*n^2-23*n+67)*a(n-1) + (5*n^2-58*n+179)*a(n-2)
 *            + (n^2-51*n+128)*a(n-3) + (-17*n^2+183*n-364)*a(n-4) -
 *            4*(n-4)*(n-11)*a(n-5) + 12*(n-4)*(n-5)*a(n-6), a(6)=3, a(7)=53,
 *            a(8)=680, a(9)=8064, a(10)=96370, a(11)=1200070. Positive and
 *            negative terms are summed separately; fixed-width bu...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   r1279_coef, r1279_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/r1279_coef.h"
#include "../utils/r1279_is_fixed.h"

#define R1279_L 16
#define R1279_MAXN 90

static void r1279_addmul(uint32_t *acc, const uint32_t *x, uint32_t m) {
    uint64_t carry = 0;
    int k;
    for (k = 0; k < R1279_L; k++) {
        uint64_t t = (uint64_t)acc[k] + (uint64_t)x[k] * m + carry;
        acc[k] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void r1279_sub(uint32_t *a, const uint32_t *b) {
    uint64_t borrow = 0;
    int k;
    for (k = 0; k < R1279_L; k++) {
        uint64_t t = (uint64_t)a[k] - (uint64_t)b[k] - borrow;
        a[k] = (uint32_t)t;
        borrow = (t >> 63) & 1;
    }
}

static void r1279_divs(uint32_t *x, uint32_t d) {
    uint64_t rem = 0;
    int k;
    for (k = R1279_L - 1; k >= 0; k--) {
        uint64_t cur = (rem << 32) | x[k];
        x[k] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static const unsigned long long r1279_init[6] = {3ULL, 53ULL, 680ULL, 8064ULL, 96370ULL, 1200070ULL};

static seq_t r1279_fixed(idx_t n) {
    uint32_t A[R1279_MAXN + 1][R1279_L];
    uint32_t P[R1279_L], N[R1279_L];
    idx_t i;
    int j, k;
    seq_t v;
    if (n > R1279_MAXN) return 0;
    for (i = 0; i <= R1279_MAXN; i++)
        for (k = 0; k < R1279_L; k++) A[i][k] = 0;
    for (j = 0; j < 6; j++) {
        A[6 + j][0] = (uint32_t)(r1279_init[j] & 0xffffffffULL);
        A[6 + j][1] = (uint32_t)(r1279_init[j] >> 32);
    }
    for (i = 12; i <= n; i++) {
        for (k = 0; k < R1279_L; k++) { P[k] = 0; N[k] = 0; }
        for (j = 1; j <= 6; j++) {
            long long c = r1279_coef((long long)i, j);
            if (c > 0) r1279_addmul(P, A[i - j], (uint32_t)c);
            else if (c < 0) r1279_addmul(N, A[i - j], (uint32_t)(-c));
        }
        r1279_sub(P, N);
        r1279_divs(P, (uint32_t)(3 * (i - 6)));
        for (k = 0; k < R1279_L; k++) A[i][k] = P[k];
    }
    v = 0;
    for (k = 15; k >= 0; k--) {
        uint32_t byte = (A[n][k / 4] >> (8 * (k % 4))) & 0xffu;
        v = (seq_t)(v * (seq_t)256);
        v = (seq_t)(v + (seq_t)byte);
    }
    return v;
}

static seq_t *r1279_memo = NULL;
static idx_t r1279_done = 0; /* memo valid for indices < r1279_done */
static idx_t r1279_cap = 0;

static seq_t r1279_big(idx_t n) {
    idx_t i;
    int j;
    if (r1279_memo == NULL) {
        r1279_cap = 64;
        r1279_memo = (seq_t *)malloc(r1279_cap * sizeof(seq_t));
        for (i = 0; i < r1279_cap; i++) r1279_memo[i] = 0;
        for (j = 0; j < 6; j++) r1279_memo[6 + j] = (seq_t)r1279_init[j];
        r1279_done = 12;
    }
    if (n >= r1279_cap) {
        idx_t nc = n + 64;
        seq_t *nm = (seq_t *)malloc(nc * sizeof(seq_t));
        for (i = 0; i < nc; i++) nm[i] = 0;
        for (i = 0; i < r1279_done; i++) nm[i] = r1279_memo[i];
        free(r1279_memo);
        r1279_memo = nm;
        r1279_cap = nc;
    }
    for (i = r1279_done; i <= n; i++) {
        seq_t P = 0, N = 0;
        for (j = 1; j <= 6; j++) {
            long long c = r1279_coef((long long)i, j);
            if (c > 0) P += (seq_t)(unsigned long long)c * r1279_memo[i - j];
            else if (c < 0) N += (seq_t)(unsigned long long)(-c) * r1279_memo[i - j];
        }
        P -= N;
        r1279_memo[i] = P / ((seq_t)3 * ((seq_t)i - 6));
        r1279_done = i + 1;
    }
    return r1279_memo[n];
}

seq_t a(idx_t n) {
    if (n < 6) return 0;
    if (n < 12) return (seq_t)r1279_init[n - 6];
    if (r1279_is_fixed()) return r1279_fixed(n);
    return r1279_big(n);
}
