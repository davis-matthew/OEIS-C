/*
 * A006442  Expansion of 1/sqrt(1 - 10*x + x^2).
 * https://oeis.org/A006442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} ... computed as: n*a(n) = 5*(2n-1)*a(n-1) -
 *            (n-1)*a(n-2), a(0) = 1, a(1) = 5 (arbitrary precision); for
 *            fixed-width builds, the lattice-path count D(n,n) with D(i,j) =
 *            2*D(i-1,j) + D(i,j-1) + D(i-1,j-1), D(0,j) = 1, D(i,0) = 2^i,
 *            which is exact modulo 2^w
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static seq_t a_fixed(idx_t n) {
    seq_t *row = alloc_zero(n + 1);
    idx_t i, j;
    seq_t res;
    for (j = 0; j <= n; j++) row[j] = 1;
    for (i = 1; i <= n; i++) {
        seq_t prev_old = row[0];          /* D(i-1, 0) */
        row[0] = row[0] * 2;              /* D(i, 0) = 2^i */
        for (j = 1; j <= n; j++) {
            seq_t old = row[j];           /* D(i-1, j) */
            row[j] = 2 * old + row[j - 1] + prev_old;
            prev_old = old;
        }
    }
    res = row[n];
    release(row, n + 1);
    return res;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n >= memo_len) {
        idx_t old_len = memo_len;
        idx_t new_len = n + 1;
        idx_t i;
        memo = grow_zero(memo, old_len, new_len);
        if (old_len < 2) {
            memo[0] = 1;
            memo[1] = 5;
            old_len = 2;
        }
        for (i = old_len; i < new_len; i++) {
            seq_t si = (seq_t)i;
            memo[i] = (5 * (2 * si - 1) * memo[i - 1] - (si - 1) * memo[i - 2]) / si;
        }
        memo_len = new_len;
    }
    return memo[n];
}
