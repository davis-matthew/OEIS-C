/*
 * A002050  Number of simplices in barycentric subdivision of n-simplex.
 * https://oeis.org/A002050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2*A000670(n) - 1 for n >= 1, a(0) = 0, where A000670(n) =
 *            Sum_{k=1..n} k!*S2(n,k) and T(n,k) = k!*S2(n,k) satisfies T(n,k)
 *            = k*(T(n-1,k) + T(n-1,k-1)), T(0,0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"

static seq_t *row = NULL;
static idx_t row_cap = 0;
static idx_t row_n = 0;

seq_t a(idx_t n) {
    idx_t i, k;
    seq_t f;
    if (n == 0) return 0;
    if (row == NULL) {
        row_cap = n + 1;
        row = a001028_z(row_cap);
        row[0] = 1;
        row_n = 0;
    } else if (n + 1 > row_cap) {
        row = a001028_grow(row, row_cap, n + 1);
        row_cap = n + 1;
    }
    if (n < row_n) {
        for (k = 0; k < row_cap; k++) row[k] = 0;
        row[0] = 1;
        row_n = 0;
    }
    for (i = row_n + 1; i <= n; i++) {
        for (k = i; k >= 1; k--)
            row[k] = (seq_t)k * (row[k] + row[k - 1]);
        row[0] = 0;
    }
    row_n = n;
    f = 0;
    for (k = 1; k <= n; k++) f += row[k];
    return f + f - 1;
}
