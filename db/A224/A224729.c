/*
 * A224729  Central terms of the triangle in A059283.
 * https://oeis.org/A224729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A059283(2n,n), where T(0,0)=1, T(r,0)=0 for r>0, T(r,k) =
 *            T(r,k-1) + T(r-1,k-1) + T(r-1,k) + T(r-2,k-1), with T = 0 for k
 *            > r
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t w = (size_t)n + 2;
    seq_t *b0 = mem_new(seq_t, w);
    seq_t *b1 = mem_new(seq_t, w);
    seq_t *b2 = mem_new(seq_t, w);
    seq_t *p2 = b0, *p1 = b1, *cur = b2;
    idx_t R = 2 * n;
    for (idx_t r = 0; r <= R; r++) {
        cur[0] = (seq_t)(r == 0 ? 1 : 0);
        idx_t kmax = r < n ? r : n;
        for (idx_t k = 1; k <= kmax; k++) {
            seq_t s = cur[k - 1] + p1[k - 1];
            if (k <= r - 1) s += p1[k];
            if (r >= 2 && k - 1 <= r - 2) s += p2[k - 1];
            cur[k] = s;
        }
        seq_t *t = p2;
        p2 = p1;
        p1 = cur;
        cur = t;
    }
    seq_t res = p1[n];
    mem_free(b0);
    mem_free(b1);
    mem_free(b2);
    return res;
}
