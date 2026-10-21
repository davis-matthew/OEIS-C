/*
 * A061546  Harmonic mean of digits is 7.
 * https://oeis.org/A061546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A061546
    Harmonic mean of digits equals 7
    (sum of 7/di = number of digits).
    This code enumerates all such integers in increasing order
    by depth‑first search using a pruned weight recurrence.
    The sequence is generated lazily and memoised.
    No external libraries are required. */

#include <stdbool.h>
#include <stdlib.h>

/* Integer weight of each digit (scaled by 2520):
   w(d) = 7/d – 1  =>  2520*(7/d – 1)  */
static const int weight[10] = {0, 15120, 6300, 3360, 1890,
                               1008, 420, 0, -315, -560};

static const int w_min = weight[9];   /* most negative weight  */
static const int w_max = weight[1];   /* most positive weight  */

/* Dynamic array for memoisation (c‑style). */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static size_t seq_count(void) { return seq_len; }

static void add_val(seq_t v) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);  /* allocation failure */
        seq_arr = tmp;
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = v;
}

/* Depth‑first search generating all numbers of length "len".
   pos   – current position (0‑based)
   val   – numeric prefix built so far
   wgt   – cumulative weight of the prefix */
static void dfs_gen(idx_t len, idx_t pos, seq_t val, int wgt) {
    if (pos == len) {
        if (wgt == 0) add_val(val);
        return;
    }
    long long rem = len - pos - 1;          /* digits still to place */
    for (int d = 1; d <= 9; ++d) {
        int new_wgt = wgt + weight[d];
        long long min_pos = rem * (long long)w_min;
        long long max_pos = rem * (long long)w_max;
        if ((long long)new_wgt + min_pos <= 0 &&
            (long long)new_wgt + max_pos >= 0) {
            dfs_gen(len, pos + 1, val * 10 + d, new_wgt);
        }
    }
}

/* Return the n-th element of the sequence (n >= 1). */
seq_t a(idx_t n) {
    static idx_t processed_len = 0; /* maximum length already generated */

    while (seq_count() < (size_t)n) {
        ++processed_len;                 /* explore next length */
        dfs_gen(processed_len, 0, 0, 0);
    }
    return seq_arr[n - 1];
}
