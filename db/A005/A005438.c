/*
 * A005438  Column of Kempner tableau.
 * https://oeis.org/A005438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = E(n+1, k) with k = (n+3)/2 for odd n and k = n/2 for even
 *            n, where E(r,k) is the Entringer (Seidel-Entringer-Arnold)
 *            triangle: E(0,0) = 1, E(r,0) = 0 for r > 0, E(r,k) = E(r,k-1) +
 *            E(r-1,r-k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cur = 0, *nxt = 0, *vals = 0;
static idx_t cap = 0, R = 0;

seq_t a(idx_t n)
{
    if (R == 0) {
        cap = 16;
        cur = alloc_zero(cap);
        nxt = alloc_zero(cap);
        vals = alloc_zero(cap);
        cur[0] = 0;
        cur[1] = 1;
        R = 1;
    }
    while (R < n + 1) {
        idx_t r = R + 1, k, m;
        seq_t *t;
        if (r + 1 > cap) {
            idx_t nc = cap * 2;
            while (nc < r + 1) nc *= 2;
            cur = grow_zero(cur, cap, nc);
            nxt = grow_zero(nxt, cap, nc);
            vals = grow_zero(vals, cap, nc);
            cap = nc;
        }
        nxt[0] = 0;
        for (k = 1; k <= r; k++)
            nxt[k] = nxt[k - 1] + cur[r - k];
        m = r - 1;
        k = (m & 1) ? (m + 3) / 2 : m / 2;
        vals[m] = nxt[k];
        t = cur; cur = nxt; nxt = t;
        R = r;
    }
    return vals[n];
}
