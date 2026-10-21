/*
 * A212117  Number of partitions of n into exactly 6 different parts with
 *          distinct multiplicities.
 * https://oeis.org/A212117
 *
 * Offset:    56    (a(56) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 85, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   min_possible_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of all 6‑tuple partitions with distinct part sizes and distinct multiplicities.
   Counts are accumulated for every sum up to the largest requested n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/min_possible_sum.h"

/* seq_t and idx_t are defined by the build system. */
static seq_t *counts = NULL;   /* counts[sum] = number of partitions of sum */
static bool *used_m = NULL;    /* used_m[m] == true if multiplicity m already chosen */
static idx_t   max_n   = 0;     /* largest n for which counts have been built */

/* Depth‑first search over all admissible 6‑tuples of (size, multiplicity).
   last_size is the last chosen part size (must increase), sum_so_far is the current sum. */
static void dfs(idx_t depth, idx_t last_size, idx_t sum_so_far)
{
    if (depth == 6)
    {
        if (sum_so_far <= max_n)
            ++counts[sum_so_far];
        return;
    }

    idx_t rem = 6 - depth;   /* pairs still to choose */

    for (idx_t s = last_size + 1; s <= max_n; ++s)
    {
        /* Maximum multiplicity that keeps the total within bounds. */
        idx_t max_m = (max_n - sum_so_far) / s;
        if (max_m == 0) break;                     /* no multiplicity fits */

        for (idx_t m = 1; m <= max_m; ++m)
        {
            if (used_m[m]) continue;                /* multiplicity already used */

            idx_t new_sum = sum_so_far + s * m;
            if (new_sum > max_n) break;

            if (depth + 1 < 6)                       /* more pairs to follow */
            {
                idx_t min_rem = min_possible_sum(6 - (depth + 1), s);
                if (new_sum + min_rem > max_n) continue;  /* prune */
            }

            used_m[m] = true;
            dfs(depth + 1, s, new_sum);
            used_m[m] = false;
        }
    }
}

/* Build counts array up to n = up (inclusive). */
static void build_counts(idx_t up)
{
    free(counts);   counts = NULL;
    free(used_m);   used_m = NULL;

    max_n = up;
    counts = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
    used_m = (bool *)calloc((size_t)(max_n + 1), sizeof(bool));

    dfs(0, 0, 0);
}

/* Public sequence generator: a(n).  Undefined for n < 56. */
seq_t a(idx_t n)
{
    if (n < 56) return 0;

    if (n > max_n) build_counts(n);
    return counts[n];
}
