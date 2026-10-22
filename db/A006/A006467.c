/*
 * A006467  Continued fraction for Sum_{n>=0} (-1)^n/3^(2^n).
 * https://oeis.org/A006467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) = n-th partial quotient of the continued fraction of x =
 *            Sum_{i>=0} (-1)^i/3^(2^i). Computed by running the Euclidean
 *            algorithm on the exact partial sum S_K = N_K/3^(2^K), where N_0
 *            = 1 and N_{i+1} = N_i*3^(2^i) + (-1)^(i+1). Only the first half
 *            of the quotients of S_K are used, because thos...
 * Range:     u8 n <= 9354, u16 n <= 9354, u32 n <= 9354, u64 n <= 9354,
 *            u128 n <= 9354, big any n
 * Helpers:   bn_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/bn_trim.h"

static unsigned int *cf_terms = NULL;
static size_t cf_trust = 0;
static unsigned cf_K = 0;

static uint32_t *bn_mul(const uint32_t *a, size_t la, const uint32_t *b, size_t lb, size_t *lo) {
    size_t i, j;
    uint32_t *r = (uint32_t *)calloc(la + lb + 2, sizeof(uint32_t));
    for (i = 0; i < la; i++) {
        uint64_t carry = 0;
        for (j = 0; j < lb; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + r[i + j] + carry;
            r[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        j = i + lb;
        while (carry) {
            uint64_t t = (uint64_t)r[j] + carry;
            r[j] = (uint32_t)t;
            carry = t >> 32;
            j++;
        }
    }
    *lo = bn_trim(r, la + lb + 2);
    return r;
}

static int bn_cmp(const uint32_t *a, size_t la, const uint32_t *b, size_t lb) {
    size_t i;
    if (la != lb) return la < lb ? -1 : 1;
    for (i = la; i > 0; i--) {
        if (a[i - 1] != b[i - 1]) return a[i - 1] < b[i - 1] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b */
static size_t bn_sub(uint32_t *a, size_t la, const uint32_t *b, size_t lb) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < la; i++) {
        int64_t t = (int64_t)a[i] - borrow - (i < lb ? (int64_t)b[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
    return bn_trim(a, la);
}

static void build(unsigned K) {
    size_t ld = 1, ln = 1, i, cap, cnt;
    uint32_t *D = (uint32_t *)calloc(2, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(2, sizeof(uint32_t));
    uint32_t *x, *y;
    size_t lx, ly;
    unsigned k;
    int stopped = 0;
    D[0] = 3;
    N[0] = 1;
    for (k = 0; k < K; k++) {
        size_t lnn, ldd;
        uint32_t *NN = bn_mul(N, ln, D, ld, &lnn);
        uint32_t *DD;
        if ((k + 1) % 2 == 0) {
            i = 0;
            while (1) {
                NN[i]++;
                if (NN[i] != 0) break;
                i++;
            }
            if (i + 1 > lnn) lnn = i + 1;
        } else {
            i = 0;
            while (NN[i] == 0) { NN[i] = 0xFFFFFFFFu; i++; }
            NN[i]--;
            lnn = bn_trim(NN, lnn);
        }
        DD = bn_mul(D, ld, D, ld, &ldd);
        free(N); free(D);
        N = NN; ln = lnn;
        D = DD; ld = ldd;
    }
    x = (uint32_t *)calloc(ld + 1, sizeof(uint32_t));
    memcpy(x, N, ln * sizeof(uint32_t));
    lx = ln;
    y = D;
    ly = ld;
    free(N);

    cap = 1024;
    cnt = 0;
    free(cf_terms);
    cf_terms = (unsigned int *)malloc(cap * sizeof(unsigned int));
    while (1) {
        unsigned int q = 0;
        uint32_t *tp;
        size_t tl;
        while (bn_cmp(x, lx, y, ly) >= 0) {
            lx = bn_sub(x, lx, y, ly);
            q++;
            if (q > 100000u) { stopped = 1; break; }
        }
        if (stopped) break;
        if (cnt == cap) {
            cap *= 2;
            cf_terms = (unsigned int *)realloc(cf_terms, cap * sizeof(unsigned int));
        }
        cf_terms[cnt++] = q;
        if (lx == 0) break;
        tp = x; x = y; y = tp;
        tl = lx; lx = ly; ly = tl;
    }
    free(x); free(y);
    cf_trust = cnt / 2;
    cf_K = K;
}

seq_t a(idx_t n) {
    unsigned K = cf_K ? cf_K : 4;
    while (cf_terms == NULL || (size_t)n >= cf_trust) {
        if (cf_terms != NULL) K++;
        build(K);
    }
    return (seq_t)cf_terms[n];
}
