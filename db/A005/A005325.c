/*
 * A005325  Column of Motzkin triangle.
 * https://oeis.org/A005325
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*sum(j=ceiling((n-5)/2)..(n+1),
 *            C(j,2*j-n+5)*C(n+1,j))/(n+1), evaluated as a(n) = 6/(n+1) *
 *            sum_{i=0..floor(m/2)} t(i), m = n-5, t(0) = C(n+1,6), t(i+1) =
 *            t(i)*(m-2i)*(m-2i-1)/((i+1)*(i+7)). For fixed-width seq_t (no
 *            exact division possible after wraparound) it instead counts
 *            Motzkin path...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1000 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t a_paths(idx_t n) {
    seq_t *cur = mem_new(seq_t, (size_t)n + 8);
    seq_t *nxt = mem_new(seq_t, (size_t)n + 8);
    seq_t r;
    idx_t s, h;
    cur[0] = 1;
    for (s = 1; s <= n; s++) {
        idx_t lim = s;
        seq_t *tmp;
        if (5 + (n - s) < lim) lim = 5 + (n - s);
        for (h = 0; h <= lim; h++) {
            seq_t v = cur[h] + cur[h + 1];
            if (h > 0) v += cur[h - 1];
            nxt[h] = v;
        }
        tmp = cur; cur = nxt; nxt = tmp;
    }
    r = cur[5];
    mem_free(cur);
    mem_free(nxt);
    return r;
}

static seq_t a_sum(idx_t n) {
    idx_t m = n - 5;
    idx_t i, r;
    seq_t t = 1, sum = 0;
    for (r = 1; r <= 6; r++)
        t = t * (seq_t)(n + 2 - r) / (seq_t)r;
    for (i = 0; 2 * i <= m; i++) {
        sum += t;
        if (2 * i + 1 >= m) break;
        t = t * (seq_t)(m - 2 * i) * (seq_t)(m - 2 * i - 1)
              / ((seq_t)(i + 1) * (seq_t)(i + 7));
    }
    return sum * 6 / (seq_t)(n + 1);
}

seq_t a(idx_t n) {
    if (n < 5) return 0;
    if (seq_is_big()) return a_sum(n);
    return a_paths(n);
}
