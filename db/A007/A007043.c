/*
 * A007043  Number of noncommutative SL(2,C)-invariants of degree n in 5
 *          variables.
 * https://oeis.org/A007043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   D-finite recurrence 2*n*(2*n+1)*(3*n-5)*a(n) =
 *            (n-1)*(3*n-2)*(19*n-20)*a(n-1) + 10*(n-1)*n*(3*n-5)*a(n-2) -
 *            25*(n-2)*(n-1)*(3*n-2)*a(n-3), a(0) = 1, a(1) = 0, a(2) = 1
 *            (arbitrary precision). For fixed-width seq_t, a(n) = number of
 *            closed walks of length n from spin 0 in the SU(2) Clebsch-Gordan
 *            g...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 32, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 910 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

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

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *memo = 0;
static idx_t memolen = 0;

static void fill_big(idx_t from, idx_t upto) {
    idx_t k;
    for (k = from; k <= upto; k++) {
        if (k < 3) {
            memo[k] = (k == 1) ? (seq_t)0 : (seq_t)1;
        } else {
            seq_t N = (seq_t)k;
            seq_t lhs = 2 * N * (2 * N + 1) * (3 * N - 5);
            seq_t t1 = (N - 1) * (3 * N - 2) * (19 * N - 20) * memo[k - 1];
            seq_t t2 = 10 * (N - 1) * N * (3 * N - 5) * memo[k - 2];
            seq_t t3 = 25 * (N - 2) * (N - 1) * (3 * N - 2) * memo[k - 3];
            memo[k] = (t1 + t2 - t3) / lhs;
        }
    }
}

static void fill_walks(idx_t upto) {
    idx_t sz = 2 * upto + 5;
    seq_t *cur = alloc_zero(sz);
    seq_t *nxt = alloc_zero(sz);
    seq_t *tmp;
    idx_t k, jp, j, hi, lo;
    cur[0] = 1;
    memo[0] = 1;
    for (k = 1; k <= upto; k++) {
        hi = 2 * k;
        if (2 * (upto - k) < hi) hi = 2 * (upto - k);
        for (jp = 0; jp <= hi; jp++) {
            seq_t s = 0;
            lo = (jp >= 2) ? jp - 2 : 0;
            for (j = lo; j <= jp + 2; j++) {
                if (j + jp >= 2) s += cur[j];
            }
            nxt[jp] = s;
        }
        nxt[hi + 1] = 0;
        nxt[hi + 2] = 0;
        tmp = cur; cur = nxt; nxt = tmp;
        memo[k] = cur[0];
    }
    release(cur, sz);
    release(nxt, sz);
}

seq_t a(idx_t n) {
    if (n < memolen) return memo[n];
    memo = grow_zero(memo, memolen, n + 1);
    if (seq_is_big()) {
        fill_big(memolen, n);
    } else {
        fill_walks(n);
    }
    memolen = n + 1;
    return memo[n];
}
