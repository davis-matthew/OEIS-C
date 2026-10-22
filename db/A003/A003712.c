/*
 * A003712  Expansion of e.g.f. sin(sin(x)) (odd powers only).
 * https://oeis.org/A003712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * Sum_{k odd} T(2n+1,k), where T(N,k) = T(N-2,k-2)
 *            + k^2*T(N-2,k), T(1,1) = 1, T(N,k) = 0 otherwise (T(N,k) =
 *            number of partitions of an N-set into k odd blocks, e.g.f.
 *            sinh(x)^k/k!)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *row = NULL;   /* row[j] = T(2m+1, 2j+1) for current m */
static seq_t *res = NULL;   /* memoized a(m) */
static idx_t cap = 0;
static idx_t nres = 0;

seq_t a(idx_t n) {
    if (n < nres) return res[n];
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        row = grow_zero(row, cap, nc);
        res = grow_zero(res, cap, nc);
        cap = nc;
    }
    if (nres == 0) {
        row[0] = 1;
        res[0] = 1;
        nres = 1;
    }
    for (idx_t m = nres; m <= n; m++) {
        seq_t s = 0;
        for (idx_t j = m; j >= 1; j--) {
            seq_t k = (seq_t)(2 * j + 1);
            row[j] = row[j - 1] + k * k * row[j];
            s += row[j];
        }
        s += row[0];
        res[m] = (m & 1) ? -s : s;
    }
    nres = n + 1;
    return res[n];
}
