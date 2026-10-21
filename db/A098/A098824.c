/*
 * A098824  Array read by antidiagonals: Minimizing absolutely ordered
 *          sequences of m-ary Huffman trees of maximum height; m > 1.
 * https://oeis.org/A098824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   G_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098824
   a(n) is the entry obtained by reading the array
   T[m,k] (m ≥ 2, k ≥ 0) by antidiagonals.
   T[m,0] = G[0,m-1]
   T[m,(i‑1)*(m‑1)+j] = G[i,m‑1]   (i > 0, 1 ≤ j ≤ m‑1)
   G[0,m] = G[1,m] = 1,  G[2,m] = 2,
   G[n,m] = G[n‑1,m] + m·G[n‑2,m]   (n > 2) */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/G_value.h"

seq_t a(idx_t n)
{
    /* Determine the diagonal s such that
       cumulative_before(s) <= n < cumulative_before(s)+s
       cumulative_before(s) = (s-1)*s/2  */
    idx_t s = 1;
    idx_t cum_before = 0;
    while (cum_before + s <= n) {
        cum_before += s;
        ++s;
    }

    /* offset within this diagonal (0‑based) */
    idx_t offset = n - cum_before;          /* 0 ≤ offset < s */
    idx_t m = offset + 2;                    /* m ≥ 2 */
    idx_t k = s - offset - 1;                /* k ≥ 0 */

    if (k == 0) {
        /* T[m,0] = G[0,m-1] = 1 */
        return 1;
    }

    idx_t mminus1 = m - 1;
    idx_t i = ((k - 1) / mminus1) + 1;       /* n‑index into G */

    return G_value(i, mminus1);
}
