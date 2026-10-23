/*
 * A171502  Irregular triangle T(n, k) = coefficients of p(x, n), where p(x,
 *          n) = ((1-x^(2*n))/(1-x))*p(x, n-1), p(x, 0) = 1, and p(x, 1) =
 *          (1-x^4)/(1-x), read by rows.
 * https://oeis.org/A171502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient at position k in the irregular triangle
 *            T(r,k) where r is the row index such that Σ_{i=1}^{r-1} i² < n ≤
 *            Σ_{i=1}^r i² T(r,k) = coefficient of ∏_{j=2}^{r} (1 + x + … +
 *            x^{2j-1}) at x^k
 * Range:     u8 n <= 62, u16 n <= 158, u32 n <= 550, u64 n <= 2218,
 *            u128 n <= 2870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    size_t len;
    seq_t *data;
} row_t;

static row_t rows[64];
static idx_t rows_count = 0;
static int initialized = 0;

/* Construct the first row: r = 1, length 1, coefficient [1] */
static void init_rows(void) {
    rows_count = 1;
    rows[1].len = 1;
    rows[1].data = (seq_t*)malloc(sizeof(seq_t));
    rows[1].data[0] = (seq_t)1;
    initialized = 1;
}

seq_t a(idx_t n) {
    /* Ensure initialization */
    if (!initialized) init_rows();

    /* Find the row r such that Σ_{i=1}^{r-1} i² < n ≤ Σ_{i=1}^{r} i² */
    idx_t r = 1;
    idx_t sum_sq = 1;                 /* Σ_{i=1}^{r} i²  */
    while (sum_sq < n) {
        ++r;
        sum_sq += r * r;
    }

    /* Build rows up to r if not already built */
    while (rows_count < r) {
        idx_t cur = rows_count + 1;           /* new row index */
        size_t prev_len = rows[cur - 1].len;
        size_t factor_len = 2 * cur;           /* length of new factor (all ones) */
        size_t new_len = prev_len + factor_len - 1;

        seq_t *new_data = (seq_t*)malloc(new_len * sizeof(seq_t));
        for (size_t k = 0; k < new_len; ++k) {
            seq_t sum = 0;
            size_t i_min = (k >= factor_len - 1) ? k - (factor_len - 1) : 0;
            size_t i_max = (k < prev_len) ? k : prev_len - 1;
            for (size_t i = i_min; i <= i_max; ++i) {
                sum += rows[cur - 1].data[i];
            }
            new_data[k] = sum;
        }

        rows[cur].len = new_len;
        rows[cur].data = new_data;
        rows_count = cur;
    }

    /* Determine position within the row */
    idx_t sum_prev = sum_sq - r * r;        /* Σ_{i=1}^{r-1} i² */
    idx_t pos = n - sum_prev - 1;           /* zero‑based index in row r */

    return rows[r].data[(size_t)pos];
}
