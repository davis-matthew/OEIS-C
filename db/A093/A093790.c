/*
 * A093790  Hook products of all partitions of 11.
 * https://oeis.org/A093790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 11! / A003875(57-n) (sorted hook products of all
 *            partitions of 11)
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: full, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Compute the hook product for a single partition.  The partition
   is stored in part[0 .. len-1] in non‑increasing order. */
static seq_t partition_hook(const int *part, size_t len)
{
    int col_height[11] = {0};

    /* Count column heights. */
    for (size_t r = 0; r < len; ++r) {
        int row_len = part[r];
        for (int c = 0; c < row_len; ++c) {
            ++col_height[c];
        }
    }

    /* Multiply hook lengths. */
    seq_t prod = 1;
    for (size_t r = 0; r < len; ++r) {
        int row_len = part[r];
        for (int c = 0; c < row_len; ++c) {
            int hook = (row_len - c) + (col_height[c] - (int)r) - 1;
            prod *= (seq_t)hook;
        }
    }
    return prod;
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Global scratch storage for the 56 hook products. */
static seq_t hooks[56];
static size_t hook_cnt = 0;
static int hooks_ready = 0;

/* Comparator for qsort. */

/* Recursively generate all partitions of 'rem' with parts <= max_part.
   The current partition is stored in curr[0 .. depth-1]. */
static void generate_partitions(int rem, int max_part, int depth, int *curr)
{
    if (rem == 0) {
        hooks[hook_cnt++] = partition_hook(curr, depth);
        return;
    }
    for (int p = rem < max_part ? rem : max_part; p >= 1; --p) {
        curr[depth] = p;
        generate_partitions(rem - p, p, depth + 1, curr);
    }
}

/* Initialise the hook product table once. */
static void init_hooks(void)
{
    if (hooks_ready) return;
    int curr[11];
    generate_partitions(11, 11, 0, curr);
    mem_qsort(hooks, hook_cnt, sizeof(seq_t), seq_cmp);
    hooks_ready = 1;
}

/* Public function returning the n‑th value of the sequence.
   Defined for 1 ≤ n ≤ 56; others return 0. */
seq_t a(idx_t n)
{
    if (n < 1 || n > 56) return (seq_t)0;
    init_hooks();
    return hooks[n - 1];
}
