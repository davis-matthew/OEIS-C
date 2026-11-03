/*
 * A007840  Number of factorizations of permutations of n letters into ordered
 *          cycles.
 * https://oeis.org/A007840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k! * s(n, k), s(n,k) = unsigned Stirling
 *            numbers of the first kind, s(n,k) = s(n-1,k-1) + (n-1)*s(n-1,k),
 *            s(0,0) = 1; the sum is evaluated by Horner's rule: a(n) =
 *            (...((s(n,n))*n + s(n,n-1))*(n-1) + ... + s(n,1))*1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 414 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
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

static seq_t *srow = NULL;   /* srow[k] = s(m, k), k = 0..m */
static seq_t *vals = NULL;   /* vals[i] = a(i), i = 0..m */
static idx_t cap = 0;
static idx_t m = 0;

seq_t a(idx_t n) {
    if (srow == NULL) {
        cap = 64;
        srow = alloc_zero(cap);
        vals = alloc_zero(cap);
        srow[0] = 1;
        vals[0] = 1;
        m = 0;
    }
    if (n <= m) return vals[n];

    if (n + 2 > cap) {
        idx_t nc = cap;
        while (n + 2 > nc) nc *= 2;
        srow = grow_zero(srow, cap, nc);
        vals = grow_zero(vals, cap, nc);
        cap = nc;
    }

    while (m < n) {
        /* advance row m -> m+1 */
        srow[m + 1] = 0;
        for (idx_t k = m + 1; k >= 1; k--) {
            srow[k] = srow[k - 1] + srow[k] * (seq_t)m;
        }
        srow[0] = srow[0] * (seq_t)m;
        m++;

        seq_t acc = 0;
        for (idx_t k = m; k >= 1; k--) {
            acc = (acc + srow[k]) * (seq_t)k;
        }
        vals[m] = acc;
    }
    return vals[n];
}
