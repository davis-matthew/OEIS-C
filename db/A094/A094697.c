/*
 * A094697  Where records occur in A007321.
 * https://oeis.org/A094697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th position m where A007321(m) exceeds all earlier
 *            A007321 values (records, strict); found by a search over m, with
 *            A007321 simulated by big integers
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void A007321_bn_shl(uint32_t *a, size_t L, int k) {
    uint32_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint32_t v = a[i];
        a[i] = (v << k) | carry;
        carry = v >> (32 - k);
    }
}

static size_t A007321_bn_trim(const uint32_t *d, size_t n) {
    while (n > 0 && d[n - 1] == 0) n--;
    return n;
}

static uint32_t *A007321_bn_mul(const uint32_t *x, size_t nx, const uint32_t *y, size_t ny, size_t *nz) {
    size_t L = nx + ny;
    uint32_t *z = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    size_t i, j;
    for (i = 0; i < nx; i++) {
        uint64_t carry = 0;
        for (j = 0; j < ny; j++) {
            uint64_t t = (uint64_t)x[i] * y[j] + z[i + j] + carry;
            z[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        size_t k = i + ny;
        while (carry) {
            uint64_t t = (uint64_t)z[k] + carry;
            z[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }
    *nz = A007321_bn_trim(z, L);
    return z;
}

static int A007321_bn_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    size_t i = L;
    while (i > 0) {
        i--;
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static void A007321_bn_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
}

static uint32_t *A007321_bn_round_sqrt(const uint32_t *x, size_t nx, size_t *nr) {
    size_t L = nx + 2, i;
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *rem = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    long long pos;
    for (pos = (long long)(32 * nx) - 2; pos >= 0; pos -= 2) {
        A007321_bn_shl(rem, L, 2);
        rem[0] |= (x[pos >> 5] >> (pos & 31)) & 3u;
        memcpy(t, res, L * sizeof(uint32_t));
        A007321_bn_shl(t, L, 2);
        t[0] |= 1u;
        A007321_bn_shl(res, L, 1);
        if (A007321_bn_cmp(rem, t, L) >= 0) {
            A007321_bn_sub(rem, t, L);
            res[0] |= 1u;
        }
    }
    if (A007321_bn_cmp(rem, res, L) > 0) {
        for (i = 0; i < L; i++) {
            res[i]++;
            if (res[i] != 0) break;
        }
    }
    free(rem);
    free(t);
    *nr = A007321_bn_trim(res, L);
    return res;
}

/* number of steps for m to reach 1, as a native integer */
static long long A007321_steps(uint64_t m) {
    uint32_t *cur = (uint32_t *)calloc(2, sizeof(uint32_t));
    size_t nc;
    long long steps = 0;
    cur[0] = (uint32_t)m;
    cur[1] = (uint32_t)(m >> 32);
    nc = A007321_bn_trim(cur, 2);
    while (!(nc == 1 && cur[0] == 1)) {
        uint32_t *next;
        size_t nn;
        if ((cur[0] & 1u) == 0) {
            next = A007321_bn_round_sqrt(cur, nc, &nn);
        } else {
            size_t n2, n3;
            uint32_t *sq = A007321_bn_mul(cur, nc, cur, nc, &n2);
            uint32_t *cu = A007321_bn_mul(sq, n2, cur, nc, &n3);
            free(sq);
            next = A007321_bn_round_sqrt(cu, n3, &nn);
            free(cu);
        }
        free(cur);
        cur = next;
        nc = nn;
        steps++;
    }
    free(cur);
    return steps;
}

#define A094697_CAP 512

seq_t a(idx_t n) {
    static uint64_t recs[A094697_CAP];
    static size_t cnt = 0;
    static uint64_t next_m = 1;
    static long long best = -1;
    while (cnt < (size_t)n && cnt < A094697_CAP) {
        long long s = A007321_steps(next_m);
        if (s > best) {
            best = s;
            recs[cnt++] = next_m;
        }
        next_m++;
    }
    return (seq_t)recs[(size_t)n - 1];
}
