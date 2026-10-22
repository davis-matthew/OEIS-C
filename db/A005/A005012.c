/*
 * A005012  Shifts one place left under 6th-order binomial transform.
 * https://oeis.org/A005012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n) = Sum_{m=0..n} 6^(n-m)*stirling2(n,m); computed via T(n,m)
 *            = 6^(n-m)*S2(n,m), T(n,m) = 6*m*T(n-1,m) + T(n-1,m-1), T(0,0) =
 *            1, a(n) = Sum_m T(n,m)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *row = 0;     /* row[m] = T(rows_done, m) */
static seq_t *vals = 0;    /* vals[k] = a(k) for k <= rows_done */
static idx_t cap = 0;
static idx_t rows_done = 0;
static int inited = 0;

seq_t a(idx_t n) {
    if (!inited) {
        cap = 16;
        row = grow_zero(0, 0, cap);
        vals = grow_zero(0, 0, cap);
        row[0] = 1;
        vals[0] = 1;
        rows_done = 0;
        inited = 1;
    }
    if (n + 1 > cap) {
        idx_t nc = cap;
        while (nc < n + 1) nc *= 2;
        row = grow_zero(row, cap, nc);
        vals = grow_zero(vals, cap, nc);
        cap = nc;
    }
    while (rows_done < n) {
        idx_t i = rows_done + 1;
        seq_t s = 0;
        idx_t m;
        row[i] = row[i - 1];
        for (m = i - 1; m >= 1; m--) {
            row[m] = (seq_t)(6 * m) * row[m] + row[m - 1];
        }
        row[0] = 0;
        for (m = 0; m <= i; m++) s += row[m];
        vals[i] = s;
        rows_done = i;
    }
    return vals[n];
}
