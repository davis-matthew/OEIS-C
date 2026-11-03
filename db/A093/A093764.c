/*
 * A093764  Hook products of all partitions of 6.
 * https://oeis.org/A093764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the n-th smallest hook product of all partitions of 6.
 *            The 11 values are obtained by enumerating the 11 partitions of
 *            6, computing each diagram’s hook product, and sorting the
 *            results
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 11 terms through n = 11 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Hook product for a partition given by parts[0..len-1] (non‑increasing). */
static seq_t hook_product(const int parts[], int len)
{
    seq_t prod = 1;
    for (int i = 0; i < len; ++i) {
        int row_len = parts[i];
        for (int j = 0; j < row_len; ++j) {
            int right = row_len - j - 1;      /* cells to the right in the same row */
            int down  = 0;                    /* cells below in the same column */
            for (int k = i + 1; k < len; ++k)
                if (parts[k] > j) ++down;
            prod *= (seq_t)(right + down + 1);
        }
    }
    return prod;
}

/* Recursively generate all partitions of 'remaining',
   with each next part ≤ max_next. */
static void gen_partitions(int remaining,
                           int max_next,
                           int cur[],
                           int cur_len,
                           seq_t out[],
                           int *out_cnt)
{
    if (remaining == 0) {
        out[(*out_cnt)++] = hook_product(cur, cur_len);
        return;
    }
    for (int part = (remaining < max_next ? remaining : max_next); part >= 1; --part) {
        cur[cur_len] = part;
        gen_partitions(remaining - part, part, cur, cur_len + 1, out, out_cnt);
    }
}

/* Simple insertion sort for the 11 values. */
static void sort_vals(seq_t vals[], int n)
{
    for (int i = 1; i < n; ++i) {
        seq_t key = vals[i];
        int j = i - 1;
        while (j >= 0 && vals[j] > key) {
            vals[j + 1] = vals[j];
            --j;
        }
        vals[j + 1] = key;
    }
}

#define PART_COUNT 11  /* number of partitions of 6 */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t values[PART_COUNT];

    if (!initialized) {
        int cur_parts[10];
        int out_cnt = 0;
        gen_partitions(6, 6, cur_parts, 0, values, &out_cnt);
        sort_vals(values, PART_COUNT);
        initialized = true;
    }

    return (n >= 1 && n <= (idx_t)PART_COUNT) ? values[(int)(n - 1)] : (seq_t)0;
}
