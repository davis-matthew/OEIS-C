/*
 * A006326  Number of total preorders on a n-set.
 * https://oeis.org/A006326
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)*E(n)/2, where E(n) = A000111(n) are the Euler
 *            (up/down) numbers, computed with the Seidel-Entringer triangle:
 *            T(0,0) = 1, T(r,0) = 0, T(r,k) = T(r,k-1) + T(r-1,r-k), E(r) =
 *            T(r,r)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t euler_up_down(idx_t n) {
    seq_t *prev = alloc_zero(n + 1);
    seq_t *cur = alloc_zero(n + 1);
    seq_t *tmp;
    seq_t res;
    idx_t r, k;
    prev[0] = 1;
    for (r = 1; r <= n; r++) {
        cur[0] = 0;
        for (k = 1; k <= r; k++)
            cur[k] = cur[k - 1] + prev[r - k];
        tmp = prev; prev = cur; cur = tmp;
    }
    res = prev[n];
    release(prev, n + 1);
    release(cur, n + 1);
    return res;
}

seq_t a(idx_t n) {
    seq_t e = euler_up_down(n);
    if (n % 2 == 0)
        return (seq_t)((n - 2) / 2) * e;
    return (seq_t)(n - 2) * (e / 2);
}
