/*
 * A007476  Shifts 2 places left under binomial transform.
 * https://oeis.org/A007476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen, nice
 * Formula:   a(n) = Sum_{k=0..n-2} binomial(n-2,k)*a(k) for n >= 2, a(0) =
 *            a(1) = 1 (a(n+2) is the binomial transform of a(0..n), built
 *            with additions only using rows R_m[j] = R_m[j-1] + R_{m-1}[j-1],
 *            R_m[0] = a(m), so a(m+2) = R_m[m])
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 31, u128 n <= 51,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static seq_t *row = NULL;
static idx_t row_len = 0;

seq_t a(idx_t n) {
    if (n >= memo_len) {
        memo = grow_zero(memo, memo_len, n + 1);
        for (idx_t i = memo_len; i <= n; i++) {
            if (i < 2) {
                memo[i] = 1;
            } else {
                idx_t m = i - 2;
                row = grow_zero(row, row_len, m + 1);
                row_len = m + 1;
                if (m == 0) {
                    row[0] = memo[0];
                } else {
                    seq_t prev = row[0];
                    row[0] = memo[m];
                    for (idx_t j = 1; j <= m; j++) {
                        seq_t tmp = 0;
                        if (j < m) tmp = row[j];
                        row[j] = row[j - 1] + prev;
                        prev = tmp;
                    }
                }
                memo[i] = row[m];
            }
        }
        memo_len = n + 1;
    }
    return memo[n];
}
