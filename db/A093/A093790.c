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
 * Helpers:   partition_hook, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/partition_hook.h"
#include "../utils/seq_cmp.h"

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
    qsort(hooks, hook_cnt, sizeof(seq_t), seq_cmp);
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
