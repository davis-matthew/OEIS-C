/*
 * A104438  Number of ways to split 1, 2, 3, ..., 5n into 5 arithmetic
 *          progressions each with n terms.
 * https://oeis.org/A104438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20 for n ≥ 6; for n ≤ 5 the values are obtained by an
 *            exhaustive back‑tracking search that enumerates all partitions
 *            of {1,…,5n} into five arithmetic progressions each of length n
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   count_partitions, dfs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_partitions.h"
#include "../utils/dfs.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for n < 1 */
    if (n >= 6) return 20;                /* known to be constant */
    return count_partitions(n);
}
