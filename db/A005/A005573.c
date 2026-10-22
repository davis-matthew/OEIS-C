/*
 * A005573  Number of walks on cubic lattice (starting from origin and not
 *          going below xy plane).
 * https://oeis.org/A005573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy, nice
 * Formula:   (n+1)*a(n) = 2*(4*n+1)*a(n-1) - 12*(n-1)*a(n-2), a(0) = 1, a(1)
 *            = 5; for fixed-width seq_t the walk count is computed directly
 *            by a height DP (f'(h) = 4*f(h) + f(h-1) + f(h+1), h >= 0, a(n) =
 *            Sum_h f(h)), which only uses + and * so it stays exact modulo
 *            the type width
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   a_dp, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/a_dp.h"

static seq_t a_rec(idx_t n) {
    seq_t p2 = 1, p1 = 5, cur;
    idx_t k;
    if (n == 0) return p2;
    if (n == 1) return p1;
    for (k = 2; k <= n; k++) {
        cur = ((seq_t)2 * ((seq_t)4 * (seq_t)k + 1) * p1
               - (seq_t)12 * ((seq_t)k - 1) * p2) / ((seq_t)k + 1);
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

seq_t a(idx_t n) {
    if (seq_is_big()) return a_rec(n);
    return a_dp(n);
}
