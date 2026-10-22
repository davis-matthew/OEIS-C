/*
 * A005239  Irregular triangle of Section I numbers. Row n contains numbers k
 *          with 2^n < k < 2^(n+1) and phi^n(k) = 2, where phi^n means n
 *          iterations of Euler's totient function.
 * https://oeis.org/A005239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n lists the odd x with 2^n < x < 2^(n+1) and h(x) = n, where
 *            h is completely additive, h(2) = 1 and h(p) = h(p-1) for odd
 *            primes p (phi^h(x)(x) = 2 for odd x). The rows are built one
 *            after another. The primes in row n have the form 2^v*m + 1 with
 *            m in row n-v (row 0 = {1}). The composites are...
 * Range:     u8 n <= 47, u16 n <= 769, u32 n <= 4037, u64 n <= 4037,
 *            u128 n <= 4037, big any n
 * Helpers:   is_prime, push
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push.h"

#define MAXROW 62

static uint64_t *rows[MAXROW + 1];
static size_t rlen[MAXROW + 1];
static uint64_t *prs[MAXROW + 1];
static size_t plen[MAXROW + 1];
static int nrows = 0;            /* rows 0..nrows-1 are built */
static idx_t cum[MAXROW + 1];    /* cum[n] = number of terms in rows 1..n */

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, w = *(const uint64_t *)y;
    return (u > w) - (u < w);
}

static size_t uniq(uint64_t *arr, size_t len) {
    size_t i, k = 0;
    if (len == 0) return 0;
    qsort(arr, len, sizeof(uint64_t), cmp_u64);
    for (i = 1; i < len; i++)
        if (arr[i] != arr[k]) arr[++k] = arr[i];
    return k + 1;
}

static void build_row(int n) {
    uint64_t *t = NULL, *pp = NULL;
    size_t tl = 0, tc = 0, pl = 0, pc = 0;
    uint64_t lim;
    int v, i;
    size_t j, k;

    if (n == 0) {
        rows[0] = (uint64_t *)malloc(sizeof(uint64_t));
        rows[0][0] = 1;
        rlen[0] = 1;
        prs[0] = NULL;
        plen[0] = 0;
        cum[0] = 0;
        return;
    }
    lim = (uint64_t)1 << (n + 1);
    for (v = 1; v <= n; v++) {
        for (j = 0; j < rlen[n - v]; j++) {
            uint64_t c = (rows[n - v][j] << v) + 1;
            if (is_prime(c)) {
                push(&t, &tl, &tc, c);
                push(&pp, &pl, &pc, c);
            }
        }
    }
    for (i = 1; i < n; i++) {
        for (j = 0; j < plen[i]; j++) {
            uint64_t p = prs[i][j];
            for (k = 0; k < rlen[n - i]; k++) {
                uint64_t b = rows[n - i][k];
                if (b >= (lim + p - 1) / p) break;   /* p*b >= lim */
                push(&t, &tl, &tc, p * b);
            }
        }
    }
    tl = uniq(t, tl);
    pl = uniq(pp, pl);
    rows[n] = t;
    rlen[n] = tl;
    prs[n] = pp;
    plen[n] = pl;
    cum[n] = cum[n - 1] + (idx_t)tl;
}

seq_t a(idx_t n) {
    int r;
    if (n < 1) return 0;
    while (nrows == 0 || cum[nrows - 1] < n) {
        if (nrows > MAXROW) return 0;
        build_row(nrows);
        nrows++;
    }
    for (r = 1; r < nrows; r++) {
        if (cum[r] >= n) {
            idx_t pos = n - cum[r - 1] - 1;
            return (seq_t)rows[r][pos];
        }
    }
    return 0;
}
