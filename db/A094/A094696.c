/*
 * A094696  Records in A007321.
 * https://oeis.org/A094696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th record value (strict left-to-right maxima, starting
 *            with 0) of A007321(m), m = 1, 2, 3, ...; computed by scanning m
 *            upward with memoized state
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 5 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void J_bn_shl(uint32_t *a, size_t L, int k) {
    uint32_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint32_t v = a[i];
        a[i] = (v << k) | carry;
        carry = v >> (32 - k);
    }
}

static size_t J_bn_trim(const uint32_t *d, size_t n) {
    while (n > 0 && d[n - 1] == 0) n--;
    return n;
}

static uint32_t *J_bn_mul(const uint32_t *x, size_t nx, const uint32_t *y, size_t ny, size_t *nz) {
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
    *nz = J_bn_trim(z, L);
    return z;
}

static int J_bn_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    size_t i = L;
    while (i > 0) {
        i--;
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static void J_bn_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
}

static uint32_t *J_bn_round_sqrt(const uint32_t *x, size_t nx, size_t *nr) {
    size_t L = nx + 2, i;
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *rem = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    long long pos;
    for (pos = (long long)(32 * nx) - 2; pos >= 0; pos -= 2) {
        J_bn_shl(rem, L, 2);
        rem[0] |= (x[pos >> 5] >> (pos & 31)) & 3u;
        memcpy(t, res, L * sizeof(uint32_t));
        J_bn_shl(t, L, 2);
        t[0] |= 1u;
        J_bn_shl(res, L, 1);
        if (J_bn_cmp(rem, t, L) >= 0) {
            J_bn_sub(rem, t, L);
            res[0] |= 1u;
        }
    }
    if (J_bn_cmp(rem, res, L) > 0) {
        for (i = 0; i < L; i++) {
            res[i]++;
            if (res[i] != 0) break;
        }
    }
    free(rem);
    free(t);
    *nr = J_bn_trim(res, L);
    return res;
}

static long long A007321_steps(uint64_t n) {
    uint32_t *cur = (uint32_t *)calloc(2, sizeof(uint32_t));
    size_t nc;
    long long steps = 0;
    cur[0] = (uint32_t)n;
    cur[1] = (uint32_t)(n >> 32);
    nc = J_bn_trim(cur, 2);
    while (!(nc == 1 && cur[0] == 1)) {
        uint32_t *next;
        size_t nn;
        if ((cur[0] & 1u) == 0) {
            next = J_bn_round_sqrt(cur, nc, &nn);
        } else {
            size_t n2, n3;
            uint32_t *sq = J_bn_mul(cur, nc, cur, nc, &n2);
            uint32_t *cu = J_bn_mul(sq, n2, cur, nc, &n3);
            free(sq);
            next = J_bn_round_sqrt(cu, n3, &nn);
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

static long long *rec_vals = NULL;
static size_t rec_cnt = 0, rec_cap = 0;
static uint64_t scan_next = 1;
static long long rec_max = -1;

seq_t a(idx_t n) {
    while (rec_cnt <= (size_t)n) {
        long long s = A007321_steps(scan_next);
        scan_next++;
        if (s > rec_max) {
            rec_max = s;
            if (rec_cnt == rec_cap) {
                rec_cap = rec_cap ? rec_cap * 2 : 64;
                rec_vals = (long long *)realloc(rec_vals, rec_cap * sizeof(long long));
            }
            rec_vals[rec_cnt++] = s;
        }
    }
    return (seq_t)rec_vals[(size_t)n];
}
