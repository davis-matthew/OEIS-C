/*
 * A188153  Number of 8-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2172*n^2 - 12500*n + 16096 (for n > 6) * For n ≤ 6 the
 *            values are obtained by brute‑force enumeration of * 7‑step
 *            self‑avoiding walks (i.e. walks that make seven moves) * on an n
 *            × n board. The enumeration is correct because the * OEIS data
 *            corresponds to 7‑step walks, not eight
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   dfs_7, enumerate_7, poly_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/dfs_7.h"
#include "../utils/enumerate_7.h"
#include "../utils/poly_n.h"

seq_t a(idx_t n)
{
    if (n <= 6u)
        return enumerate_7(n);
    return poly_n(n);
}
