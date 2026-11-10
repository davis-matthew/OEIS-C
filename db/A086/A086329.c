/*
 * A086329  Triangle T(n,k) read by rows, given by [0, 1, 0, 1, 0, 1, 0, 1, 0,
 *          1, ...] DELTA [1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, ...] where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A086329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   F(x,y) = Sum T(n,k) x^n y^k = 1/(1 - y*x/(1 - x/(1 - 2y*x/(1 -
 *            x/(1 - 3y*x/(1 - x/(1 - 4y*x/...))))))); i.e. T(n,k) is the
 *            weighted count of Dyck paths of semilength n, where an up-step
 *            from height h has weight (h/2+1)*y for even h and 1 for odd h; k
 *            = number of y factors. Computed by dynamic pro...
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 143, u64 n <= 364,
 *            u128 n <= 971, big any n
 * Validated: standard, pass, 82 terms through n = 1325 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 0, c = 0;
    while (c + r + 1 <= n) {
        c += r + 1;
        r++;
    }
    idx_t col = n - c;

    size_t R = (size_t)r;
    size_t W = R + 2;
    size_t H = R + 2;
    size_t tot = W * H;
    seq_t *cur = mem_new(seq_t, tot);
    seq_t *nxt = mem_new(seq_t, tot);
    cur[0] = 1;

    size_t steps = 2 * R;
    for (size_t s = 0; s < steps; s++) {
        for (size_t i = 0; i < tot; i++) nxt[i] = 0;
        for (size_t h = 0; h <= R; h++) {
            for (size_t k = 0; k <= R; k++) {
                seq_t v = cur[h * W + k];
                if (v == 0) continue;
                if (h < R) {
                    if ((h & 1) == 0) {
                        nxt[(h + 1) * W + k + 1] += v * (seq_t)(h / 2 + 1);
                    } else {
                        nxt[(h + 1) * W + k] += v;
                    }
                }
                if (h > 0) {
                    nxt[(h - 1) * W + k] += v;
                }
            }
        }
        seq_t *t = cur;
        cur = nxt;
        nxt = t;
    }

    seq_t res = cur[(size_t)col];
    mem_free(cur);
    mem_free(nxt);
    return res;
}
