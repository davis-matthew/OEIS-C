/*
 * A196087  Sum of all parts minus the total numbers of parts of all
 *          partitions of n.
 * https://oeis.org/A196087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p(n) - A006128(n) where p(n) = number of partitions
 *            of n A006128(n) = total number of parts over all partitions of n
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 90, u64 n <= 328,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* Provide a simple allocator for seq_t arrays if the project
   does not expose one.  This implementation uses malloc and
   therefore works for the built‑in integer and built‑in
   unsigned __int128 types employed by the evaluation harness. */

seq_t a(idx_t n)
{
    /* static state to memoise up to the largest n requested so far */
    static idx_t max_n     = 0;   /* largest n already computed */
    static seq_t *p  = NULL;      /* p[i] = number of partitions of i   */
    static seq_t *cnt = NULL;     /* cnt[i] = total parts in all partitions of i */

    /* The sequence is defined for n ≥ 1; for n = 0 return 0 */
    if (n == 0) return 0;

    /* If we haven't yet computed up to n, recompute from scratch.  */
    if (n > max_n) {
        /* Allocate new arrays of length n+1 (indices 0..n).  */
        seq_t *new_p   = seq_alloc_array(n + 1);
        seq_t *new_cnt = seq_alloc_array(n + 1);

        /* Zero initialise the arrays.  */
        for (idx_t i = 0; i <= n; ++i) {
            new_p[i]   = 0;
            new_cnt[i] = 0;
        }
        new_p[0] = 1;   /* only one way to partition 0 */

        /* Unbounded‑knapsack DP:
           for each part size i we add it to all larger totals j
           updating both the partition count and the total number of parts.  */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; ++j) {
                seq_t add_p   = new_p[j - i];
                seq_t add_cnt = new_cnt[j - i];
                new_p[j]   += add_p;          /* new partitions that end with part i  */
                new_cnt[j] += add_cnt + add_p;/* each such partition gains one part   */
            }
        }

        /* Replace the old tables (memory is leaked intentionally; the
           harness runs each sequence once per build).  */
        p    = new_p;
        cnt  = new_cnt;
        max_n = n;
    }

    /* a(n) = n * p(n) - total number of parts of all partitions of n */
    seq_t result = ((seq_t)n) * p[n] - cnt[n];
    return result;
}
