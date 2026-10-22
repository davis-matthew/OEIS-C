/*
 * A005493  2-Bell numbers: a(n) = number of partitions of [n+1] with a
 *          distinguished block.
 * https://oeis.org/A005493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum of row n of Aitken's array (Bell triangle, A011971):
 *            T(0,0) = 1, T(r,0) = T(r-1,r-1), T(r,k) = T(r,k-1) + T(r-1,k-1);
 *            a(n) = Sum_{k=0..n} T(n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    seq_t *row = alloc_zero(n + 1);
    seq_t *nxt = alloc_zero(n + 1);
    row[0] = 1;
    for (idx_t r = 1; r <= n; r++) {
        nxt[0] = row[r - 1];
        for (idx_t k = 1; k <= r; k++)
            nxt[k] = nxt[k - 1] + row[k - 1];
        seq_t *t = row;
        row = nxt;
        nxt = t;
    }
    seq_t s = 0;
    for (idx_t k = 0; k <= n; k++)
        s += row[k];
    release(row, n + 1);
    release(nxt, n + 1);
    return s;
}
