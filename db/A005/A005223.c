/*
 * A005223  Number of Dyck paths of knight moves.
 * https://oeis.org/A005223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   a(n) is the number of knight-move Dyck paths from (0,0) to (n,0)
 *            (steps (1,+-2), (2,+-1)) that touch y = 0 only at their two
 *            endpoints. This is the series 1 - 1/A005220(x). D[x][y] counts
 *            the paths from (0,0) to (x,y) whose points after the start all
 *            have y > 0: D[x][y] = D[x-1][y-2] + D[x-1][y+2...
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 28, u64 n <= 51, u128 n <= 96,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len) return cache[n];

    idx_t N = n < 64 ? 64 : n;
    if (N < 2 * cache_len) N = 2 * cache_len;

    idx_t W = 2 * N + 6;
    seq_t *L[3];
    for (int i = 0; i < 3; i++) L[i] = alloc_zero(W);
    seq_t *res = alloc_zero(N + 1);

    for (idx_t x = 1; x + 1 <= N; x++) {
        seq_t *cur = L[x % 3];
        seq_t *p1 = L[(x + 2) % 3]; /* layer x-1 */
        seq_t *p2 = L[(x + 1) % 3]; /* layer x-2 */
        for (idx_t y = 0; y < W; y++) cur[y] = 0;
        idx_t top = 2 * x;
        idx_t cap = 2 * (N - x) + 2;
        if (top > cap) top = cap;
        for (idx_t y = 1; y <= top; y++) {
            seq_t s = 0;
            if (y >= 3) s += p1[y - 2];
            s += p1[y + 2];
            if (y >= 2) s += p2[y - 1];
            s += p2[y + 1];
            cur[y] = s;
        }
        if (x == 1) cur[2] += 1;
        if (x == 2) cur[1] += 1;
        res[x + 1] = cur[2] + p1[1];
    }

    for (int i = 0; i < 3; i++) release(L[i], W);
    if (cache) release(cache, cache_len);
    cache = res;
    cache_len = N + 1;
    return cache[n];
}
