/*
 * A079430  Marks on lexicographically earliest 11-mark optimal Golomb ruler.
 * https://oeis.org/A079430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th mark of the lexicographically earliest optimal
 *            11‑mark Golomb ruler
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   dfs_ol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/dfs_ol.h"

static seq_t marks[11];
static bool computed = false;

static void compute_marks(void)
{
    int pos[11];
    bool diff[73] = {0};

    pos[0] = 0;                            /* first mark is always 0 */
    dfs_ol(1, pos, diff);

    for (int i = 0; i < 11; ++i) marks[i] = (seq_t)pos[i];
    computed = true;
}

seq_t a(idx_t n)
{
    if (!computed) compute_marks();
    if (n == 0 || n > 11) return 0;
    return marks[n - 1];
}
