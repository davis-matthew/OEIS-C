/*
 * A154726  Triangle read by rows in which row n lists: n, in the center of
 *          the row and the pairs of prime numbers that are equidistant to n,
 *          as shown below in the example.
 * https://oeis.org/A154726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   Triangle read by rows. Row n contains all primes p < n such that
 *            (n-p) and (n+p) are prime (listed sorted ascending), then n
 *            itself, then the corresponding primes > n (sorted ascending).
 *            a(n) is the n‑th element of this triangle (OEIS offset 1)
 * Range:     u8 n <= 1843, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static tables that grow as more rows are needed. */
static idx_t *row_start      = NULL;   /* linear index of first element of row (1‑based) */
static idx_t *row_len        = NULL;   /* number of elements in the row          */
static size_t  rows_cap      = 0;      /* capacity of the tables                */
static size_t  rows_cnt      = 0;      /* number of rows already stored          */

/* Ensure tables contain all rows whose first element index <= n. */
static void ensure_rows(idx_t n)
{
    if (rows_cnt == 0) {
        /* initialise with some capacity */
        rows_cap = 64;
        row_start = (idx_t *)malloc(rows_cap * sizeof(idx_t));
        row_len   = (idx_t *)malloc(rows_cap * sizeof(idx_t));
        if (!row_start || !row_len) return;
        row_start[0] = 1;
        row_len[0]   = 1;
        rows_cnt = 1;
    }

    /* Build rows until their first element index surpasses n. */
    while (row_start[rows_cnt-1] <= n) {
        idx_t r = rows_cnt + 1;          /* row number (1‑based) */
        idx_t pair_count = 0;

        if (r > 2) {
            for (idx_t d = 1; d <= r-2; ++d) {
                if (is_prime(r-d) && is_prime(r+d))
                    ++pair_count;
            }
        }

        idx_t new_len = pair_count * 2 + 1;   /* left primes + centre + right primes */

        /* Grow tables if necessary */
        if (rows_cnt >= rows_cap) {
            rows_cap <<= 1;
            row_start = (idx_t *)realloc(row_start, rows_cap * sizeof(idx_t));
            row_len   = (idx_t *)realloc(row_len,   rows_cap * sizeof(idx_t));
            if (!row_start || !row_len) return;
        }

        row_start[rows_cnt] = row_start[rows_cnt-1] + row_len[rows_cnt-1];
        row_len[rows_cnt]   = new_len;
        ++rows_cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n < 1 */

    ensure_rows(n);

    /* Find the row that contains the n‑th element. */
    size_t r_idx = 0;
    while (r_idx + 1 < rows_cnt && row_start[r_idx+1] <= n) ++r_idx;

    idx_t row_start_pos = row_start[r_idx];
    idx_t row_len_val   = row_len[r_idx];
    idx_t k             = (row_len_val - 1) / 2;      /* number of left/right primes in the row */
    idx_t pos           = n - row_start_pos;          /* 0‑based position inside the row */
    idx_t r             = r_idx + 1;                  /* actual row number (1‑based) */

    if (pos < k && r > 2) {                      /* left side */
        idx_t left_cnt = 0;
        for (idx_t d = r-2; d > 0; --d) {
            if (is_prime(r-d) && is_prime(r+d)) {
                if (left_cnt == pos)
                    return (seq_t)(r-d);
                ++left_cnt;
            }
        }
    } else if (pos == k) {                        /* centre */
        return (seq_t)r;
    } else if (pos > k && r > 2) {                /* right side */
        idx_t right_pos = pos - k - 1;
        idx_t right_cnt = 0;
        for (idx_t d = 1; d <= r-2; ++d) {
            if (is_prime(r-d) && is_prime(r+d)) {
                if (right_cnt == right_pos)
                    return (seq_t)(r+d);
                ++right_cnt;
            }
        }
    }

    /* Fallback (should not be reached) */
    return (seq_t)r;
}
