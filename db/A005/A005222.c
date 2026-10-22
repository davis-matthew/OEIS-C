/*
 * A005222  Number of Dyck paths of knight moves.
 * https://oeis.org/A005222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   a(n) = f(n,1), where f(x,h) counts lattice paths from (0,1) to
 *            (x,h) with knight steps (1,2),(1,-2),(2,1),(2,-1) never going
 *            below h = 0 (i.e. Dyck paths of knight moves that may dip to
 *            height -1, shifted up by 1): f(x,h) = f(x-1,h-2) + f(x-1,h+2) +
 *            f(x-2,h-1) + f(x-2,h+1), f(0,1) = 1, f(0,h) = 0...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a005222_cache = NULL;
static idx_t a005222_have = 0;

static void a005222_build(idx_t N)
{
    idx_t H = N + 4;
    seq_t *r2 = alloc_zero(H);   /* row x-2 */
    seq_t *r1 = alloc_zero(H);   /* row x-1 */
    seq_t *r0 = alloc_zero(H);   /* scratch for row x */
    seq_t *res = alloc_zero(N + 1);
    idx_t x, h;

    r1[1] = 1;                   /* row 0 */
    res[0] = 1;
    for (x = 1; x <= N; x++) {
        seq_t *cur = r0;
        idx_t lim = 2 * x + 1;
        if (lim > N + 1) lim = N + 1;
        for (h = 0; h <= lim; h++) {
            seq_t v = r1[h + 2];
            v += r2[h + 1];
            if (h >= 1) v += r2[h - 1];
            if (h >= 2) v += r1[h - 2];
            cur[h] = v;
        }
        res[x] = cur[1];
        r0 = r2;
        r2 = r1;
        r1 = cur;
    }
    release(r0, H);
    release(r1, H);
    release(r2, H);
    if (a005222_cache) release(a005222_cache, a005222_have);
    a005222_cache = res;
    a005222_have = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= a005222_have) {
        idx_t N = n;
        if (N < 2 * a005222_have) N = 2 * a005222_have;
        if (N < 64) N = 64;
        a005222_build(N);
    }
    return a005222_cache[n];
}
