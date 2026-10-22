/*
 * A001404  Triangle of values of 2-d recurrence.
 * https://oeis.org/A001404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   T(0,0) = 1; T(n,0) = T(n-1,0) + T(n-1,1); T(n,k) =
 *            Sum_{j=0..k+1} T(n-k,j) for 1 <= k <= n (T(m,j) = 0 for j > m);
 *            equivalently T(n,k) = T(n-1,k-1) + T(n-k,k+1) for k >= 2. a(n)
 *            reads the triangle by rows
 * Range:     u8 n <= 36, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   alloc_seq, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t r = 0, start = 0;
    while (start + r + 1 <= n) {
        start += r + 1;
        r++;
    }
    idx_t k = n - start;

    /* P[off(i) + j] = T(i,0) + ... + T(i,j), for 0 <= j <= i */
    idx_t total = (r + 1) * (r + 2) / 2;
    seq_t *P = alloc_seq(total);
    idx_t off = 0;
    seq_t result = 0;
    for (idx_t i = 0; i <= r; i++) {
        seq_t run = 0;
        for (idx_t j = 0; j <= i; j++) {
            seq_t t;
            if (i == 0) {
                t = 1;
            } else if (j == 0) {
                idx_t m = i - 1;
                idx_t lim = m < 1 ? m : 1;
                t = P[m * (m + 1) / 2 + lim];
            } else {
                idx_t m = i - j;
                idx_t lim = (j + 1 < m) ? j + 1 : m;
                t = P[m * (m + 1) / 2 + lim];
            }
            if (i == r && j == k) result = t;
            run += t;
            P[off + j] = run;
        }
        off += i + 1;
    }
    release(P, total);
    return result;
}
