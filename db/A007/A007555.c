/*
 * A007555  Number of standard paths of length n in composition poset.
 * https://oeis.org/A007555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 1/Q(0), Q(k) = 1 - x*(2*k+1) - x^2*(k+1)*(k+2)/2/Q(k+1)
 *            (J-fraction); a(n) = weighted Motzkin paths of length n: up step
 *            weight 1, level step at height h weight 2*h+1, down step from
 *            height h+1 to h weight (h+1)*(h+2)/2. a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    idx_t H, i, h, hmax;
    seq_t *cur, *nxt, *tmp, *nm;

    if (n < memo_len) return memo[n];

    H = n / 2 + 2;
    cur = alloc_zero(H + 1);
    nxt = alloc_zero(H + 1);
    nm = alloc_zero(n + 1);

    cur[0] = 1;
    nm[0] = 1;
    for (i = 1; i <= n; i++) {
        hmax = (i < n - i) ? i : n - i;
        if (hmax > H - 1) hmax = H - 1;
        for (h = 0; h <= H; h++) {
            if (h > hmax) {
                nxt[h] = 0;
                continue;
            }
            {
                uint64_t c = (uint64_t)(h + 1) * (uint64_t)(h + 2) / 2;
                seq_t v = cur[h] * (seq_t)(2 * h + 1);
                if (h > 0) v += cur[h - 1];
                v += cur[h + 1] * (seq_t)c;
                nxt[h] = v;
            }
        }
        tmp = cur; cur = nxt; nxt = tmp;
        nm[i] = cur[0];
    }

    release(cur, H + 1);
    release(nxt, H + 1);
    if (memo) release(memo, memo_len);
    memo = nm;
    memo_len = n + 1;
    return memo[n];
}
