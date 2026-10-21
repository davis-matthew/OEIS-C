/*
 * A093769  Hook products of all partitions of 7.
 * https://oeis.org/A093769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = hook product of all partitions of 7, sorted ascending.
 *            a(n) = 5040 / A003871(16 - n); the sequence has 15 terms
 * Range:     u8 none, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   hook_compare
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/hook_compare.h"

/* Compute the hook product of a partition given in parts[0..m-1]. */
static seq_t hook_product(const int *parts, int m)
{
    /* Column lengths for hook calculation: 1‑based indexing. */
    int col_len[8] = {0};
    for (int r = 0; r < m; ++r)
        for (int c = 1; c <= parts[r]; ++c)
            ++col_len[c];

    seq_t prod = 1;
    for (int r = 0; r < m; ++r)
        for (int c = 1; c <= parts[r]; ++c)
            prod *= (seq_t)(parts[r] - c + col_len[c] - r);
    return prod;
}

/* Recursively generate all partitions of 'rem' (<=7). */
static void gen_parts(int rem, int max, int parts[], int idx,
                      seq_t *arr, size_t *cnt)
{
    if (rem == 0) {
        arr[(*cnt)++] = hook_product(parts, idx);
        return;
    }
    for (int v = (rem < max ? rem : max); v >= 1; --v) {
        parts[idx] = v;
        gen_parts(rem - v, v, parts, idx + 1, arr, cnt);
    }
}

/* External function called by the test harness. */
seq_t a(idx_t n)
{
    /* Static storage for the 15 sorted hook products. */
    static seq_t values[15];
    static size_t num_vals = 0;
    static int initialized = 0;

    if (!initialized) {
        int parts[8];
        gen_parts(7, 7, parts, 0, values, &num_vals);
        qsort(values, num_vals, sizeof(seq_t), hook_compare);
        initialized = 1;
    }

    if (n < 1 || n > num_vals)
        return 0;          /* undefined beyond the known terms */

    return values[(size_t)(n - 1)];
}
