/*
 * A006699  T(3,3n), where T(k,m) is the number of sequences a_1,...,a_m of
 *          integers 0,1,...,n with n=floor(m/k) such that the 'bumped'
 *          sequence b_1,...,b_m has exactly k of each of 0,...,n-1, where
 *          b_i=a_i + j (mod n+1) with minimal j>=0 such that b_0,...,b_i
 *          contain at most k elements equal to b_i.
 * https://oeis.org/A006699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = H(n,3n), where H(j,s) counts sequences of length s over
 *            {0..j-1} with #{x < i} >= 3i for i = 1..j-1 (the bumping never
 *            wraps, so these are 3-capacity parking functions). By first
 *            failure: H(j,s) = j^s - Sum_{i=1..j-1} Sum_{t=3(i-1)..3i-1}
 *            C(s,t) H(i,t) (j-i)^(s-t), with H(1,t) = 1 and a(0)...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static void advance_row(seq_t *row, idx_t *cur, idx_t s) {
    while (*cur < s) {
        idx_t c = *cur + 1;
        for (idx_t x = c; x >= 1; x--) row[x] += row[x - 1];
        *cur = c;
    }
}

static seq_t compute_H(idx_t j, idx_t s, idx_t K, seq_t *row, idx_t *cur, seq_t *H) {
    advance_row(row, cur, s);
    seq_t val = ipow_idx((seq_t)j, s);
    for (idx_t i = 1; i < j; i++) {
        seq_t base = (seq_t)(j - i);
        for (idx_t r = 0; r < K; r++) {
            idx_t t = K * (i - 1) + r;
            seq_t term = row[t] * H[(i - 1) * K + r];
            term *= ipow_idx(base, s - t);
            val -= term;
        }
    }
    return val;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t K = 3;
    idx_t M = K * n;
    seq_t *row = alloc_zero(M + 2);
    seq_t *H = alloc_zero(K * n + 1);
    idx_t cur = 0;
    row[0] = 1;
    for (idx_t j = 1; j < n; j++) {
        for (idx_t r = 0; r < K; r++) {
            idx_t s = K * (j - 1) + r;
            H[(j - 1) * K + r] = compute_H(j, s, K, row, &cur, H);
        }
    }
    seq_t result = compute_H(n, M, K, row, &cur, H);
    release(row, M + 2);
    release(H, K * n + 1);
    return result;
}
