/*
 * A000807  Quadratic invariants.
 * https://oeis.org/A000807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2 * Sum_{k=1..n} binomial(2*n-1,2*k-1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"

static seq_t *memo = NULL;     /* memo[0..cnt-1] = a(0..cnt-1) */
static idx_t cnt = 0;
static idx_t memo_cap = 0;

static seq_t *row = NULL;      /* Pascal row number row_n, zero padded */
static idx_t row_n = 0;
static idx_t row_cap = 0;

static void advance_row(void) {
    /* row_n -> row_n + 1, in place; row[row_n + 1] is zero before this */
    idx_t r;
    for (r = row_n + 1; r >= 1; r--) {
        row[r] += row[r - 1];
    }
    row_n++;
}

seq_t a(idx_t n) {
    idx_t m, k;
    if (n < cnt) return memo[n];

    if (memo == NULL) {
        memo_cap = n + 1;
        memo = a306_alloc0(memo_cap);
        row_cap = 2 * n + 2;
        row = a306_alloc0(row_cap);
        row[0] = 1;
        row_n = 0;
    } else {
        if (n + 1 > memo_cap) {
            memo = a306_grow(memo, memo_cap, n + 1);
            memo_cap = n + 1;
        }
        if (2 * n + 2 > row_cap) {
            row = a306_grow(row, row_cap, 2 * n + 2);
            row_cap = 2 * n + 2;
        }
    }

    for (m = cnt; m <= n; m++) {
        if (m == 0) {
            memo[0] = 1;
            continue;
        }
        while (row_n < 2 * m - 1) advance_row();
        {
            seq_t sum = 0;
            for (k = 1; k <= m; k++) {
                sum += row[2 * k - 1] * memo[m - k];
            }
            memo[m] = sum * 2;
        }
    }
    cnt = n + 1;
    return memo[n];
}
