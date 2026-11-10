/*
 * A232972  Main diagonal of array P(i,j) mentioned in A064642.
 * https://oeis.org/A232972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = P(n,n) = T(2n,n), where T is the triangle of A064642:
 *            T(r,0) = 1; T(r,c) = T(r-1,c) + T(r-1,c-1) + T(r,c-1) +
 *            T(r-2,c-1) (missing terms 0, T(r,c) = 0 for c > r). Computed
 *            with 3 rolling rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t R = 2 * N;
    seq_t *p2 = mem_new(seq_t, N + 1);
    seq_t *p1 = mem_new(seq_t, N + 1);
    seq_t *cur = mem_new(seq_t, N + 1);
    seq_t result;

    for (size_t r = 0; r <= R; ++r) {
        size_t lim = (r < N) ? r : N;
        cur[0] = 1;
        for (size_t c = 1; c <= lim; ++c) {
            seq_t a1 = 0, a4 = 0;
            if (c + 1 <= r) a1 = p1[c];
            if (r >= 2 && c + 1 <= r) a4 = p2[c - 1];
            seq_t a2 = p1[c - 1];
            seq_t a3 = cur[c - 1];
            cur[c] = a1 + a2 + a3 + a4;
        }
        if (r == R) break;
        seq_t *t = p2;
        p2 = p1;
        p1 = cur;
        cur = t;
    }
    result = cur[N];
    mem_free(p2);
    mem_free(p1);
    mem_free(cur);
    return result;
}
