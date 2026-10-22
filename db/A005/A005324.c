/*
 * A005324  Column of Motzkin triangle A026300.
 * https://oeis.org/A005324
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of paths s(0..n) with s(0)=0, s(n)=4, s(i)>=0,
 *            |s(i)-s(i-1)|<=1 (counted by dynamic programming over heights,
 *            additions only, for fixed-width seq_t); for arbitrary precision:
 *            a(n)*(n+6)*(n-4) = n*(2n+1)*a(n-1) + 3*n*(n-1)*a(n-2), a(4) = 1,
 *            a(5) = 5
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   a_rec, alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a_rec.h"

static seq_t a_dp(idx_t n) {
    idx_t len = (n + 4) / 2 + 4;
    seq_t *v = alloc_zero(len);
    v[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        idx_t lim = i;
        if (n + 4 - i < lim) lim = n + 4 - i;
        if (lim > len - 2) lim = len - 2;
        seq_t prev = 0;
        for (idx_t h = 0; h <= lim; h++) {
            seq_t tmp = v[h];
            v[h] = prev + v[h] + v[h + 1];
            prev = tmp;
        }
    }
    seq_t r = v[4];
    release(v, len);
    return r;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (seq_is_fixed_width()) return a_dp(n);
    return a_rec(n);
}
