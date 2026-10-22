/*
 * A007047  Number of chains in power set of n-set.
 * https://oeis.org/A007047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 4*A000670(n) - 1 for n > 0, a(0) = 1, where A000670(n) =
 *            Sum_{k=1..n} T(n,k), T(n,k) = k!*Stirling2(n,k) = k*(T(n-1,k) +
 *            T(n-1,k-1)), T(0,0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *row = NULL;   /* row[k] = T(row_m, k), k = 0..row_m */
static idx_t row_m = 0;
static seq_t *res = NULL;   /* res[i] = a(i), i < res_len */
static idx_t res_len = 0;

seq_t a(idx_t n) {
    if (res == NULL) {
        row = alloc_zero(1);
        row[0] = 1;
        row_m = 0;
        res = alloc_zero(1);
        res[0] = 1;
        res_len = 1;
    }
    if (n < res_len) return res[n];

    row = grow_zero(row, row_m + 1, n + 1);
    res = grow_zero(res, res_len, n + 1);

    while (row_m < n) {
        idx_t m = row_m + 1;
        idx_t k;
        seq_t sum = 0;
        for (k = m; k >= 1; k--)
            row[k] = (seq_t)k * (row[k] + row[k - 1]);
        row[0] = 0;
        row_m = m;
        for (k = 1; k <= m; k++) sum += row[k];
        res[m] = (seq_t)4 * sum - 1;
    }
    res_len = n + 1;
    return res[n];
}
