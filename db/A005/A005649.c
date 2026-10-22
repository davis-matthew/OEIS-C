/*
 * A005649  Expansion of e.g.f. (2 - e^x)^(-2).
 * https://oeis.org/A005649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} (k+1)! * S2(n,k), from e.g.f. 1/(2-e^x)^2 =
 *            Sum_{k>=0} (k+1)*(e^x-1)^k; T(n,k) = k!*S2(n,k) satisfies T(0,0)
 *            = 1, T(n,0) = 0 for n > 0, T(n,k) = k*(T(n-1,k-1) + T(n-1,k)),
 *            and a(n) = Sum_{k=0..n} (k+1)*T(n,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *row = NULL;   /* row[k] = T(m,k) for the last computed m */
static idx_t row_cap = 0;
static seq_t *vals = NULL;  /* vals[m] = a(m) */
static idx_t vals_len = 0;
static idx_t vals_cap = 0;

static void ensure_cap(idx_t need) {
    if (row == NULL) {
        idx_t c = need < 64 ? 64 : need;
        row = alloc_zero(c);
        row_cap = c;
        vals = alloc_zero(c);
        vals_cap = c;
        return;
    }
    if (need > row_cap) {
        idx_t c = row_cap * 2;
        if (c < need) c = need;
        row = grow_zero(row, row_cap, c);
        row_cap = c;
    }
    if (need > vals_cap) {
        idx_t c = vals_cap * 2;
        if (c < need) c = need;
        vals = grow_zero(vals, vals_cap, c);
        vals_cap = c;
    }
}

seq_t a(idx_t n) {
    if (n < vals_len) return vals[n];
    ensure_cap(n + 1);
    while (vals_len <= n) {
        idx_t m = vals_len;
        if (m == 0) {
            row[0] = 1;
        } else {
            for (idx_t k = m; k >= 1; k--) {
                row[k] = (seq_t)k * (row[k - 1] + row[k]);
            }
            row[0] = 0;
        }
        seq_t s = 0;
        for (idx_t k = 0; k <= m; k++) {
            s += ((seq_t)k + 1) * row[k];
        }
        vals[m] = s;
        vals_len = m + 1;
    }
    return vals[n];
}
