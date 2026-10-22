/*
 * A007321  Number of steps needed for modified juggler sequence (A094685)
 *          started at n to reach 1.
 * https://oeis.org/A007321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of iterations of x -> round(x^(1/2)) if x even,
 *            round(x^(3/2)) if x odd (A094685), started at x = n, to reach 1;
 *            simulated with an internal big integer
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bn_shl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bn_shl.h"

static size_t bn_trim(const uint32_t *d, size_t n) {
    while (n > 0 && d[n - 1] == 0) n--;
    return n;
}

static uint32_t *bn_mul(const uint32_t *x, size_t nx, const uint32_t *y, size_t ny, size_t *nz) {
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
    *nz = bn_trim(z, L);
    return z;
}

static int bn_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    size_t i = L;
    while (i > 0) {
        i--;
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static void bn_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
}

/* returns round(sqrt(x)) */
static uint32_t *bn_round_sqrt(const uint32_t *x, size_t nx, size_t *nr) {
    size_t L = nx + 2, i;
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *rem = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    long long pos;
    for (pos = (long long)(32 * nx) - 2; pos >= 0; pos -= 2) {
        bn_shl(rem, L, 2);
        rem[0] |= (x[pos >> 5] >> (pos & 31)) & 3u;
        memcpy(t, res, L * sizeof(uint32_t));
        bn_shl(t, L, 2);
        t[0] |= 1u;
        bn_shl(res, L, 1);
        if (bn_cmp(rem, t, L) >= 0) {
            bn_sub(rem, t, L);
            res[0] |= 1u;
        }
    }
    /* x = r^2 + rem; round up iff x > r^2 + r, i.e. rem > r */
    if (bn_cmp(rem, res, L) > 0) {
        for (i = 0; i < L; i++) {
            res[i]++;
            if (res[i] != 0) break;
        }
    }
    free(rem);
    free(t);
    *nr = bn_trim(res, L);
    return res;
}

seq_t a(idx_t n) {
    uint32_t *cur = (uint32_t *)calloc(2, sizeof(uint32_t));
    size_t nc;
    long long steps = 0;
    cur[0] = (uint32_t)n;
    cur[1] = (uint32_t)((uint64_t)n >> 32);
    nc = bn_trim(cur, 2);
    while (!(nc == 1 && cur[0] == 1)) {
        uint32_t *next;
        size_t nn;
        if ((cur[0] & 1u) == 0) {
            next = bn_round_sqrt(cur, nc, &nn);
        } else {
            size_t n2, n3;
            uint32_t *sq = bn_mul(cur, nc, cur, nc, &n2);
            uint32_t *cu = bn_mul(sq, n2, cur, nc, &n3);
            free(sq);
            next = bn_round_sqrt(cu, n3, &nn);
            free(cu);
        }
        free(cur);
        cur = next;
        nc = nn;
        steps++;
    }
    free(cur);
    return (seq_t)steps;
}
