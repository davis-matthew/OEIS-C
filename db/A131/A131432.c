/*
 * A131432  Triangle read by rows: T(n,k) = 3*(n + k) + 1, 0 <= k <= n.
 * https://oeis.org/A131432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A131432 – triangle read by rows
   T(n,k) = 3*(n+k)+1,  0 ≤ k ≤ n
   a(n) is the triangle read row‑wise.
   For a given index n (starting at 0):
      r = floor((sqrt(8*n + 1) – 1) / 2)        // row number
      c = n – r*(r+1)/2                        // column number within that row
      a(n) = 3*(r + c) + 1
   This implementation uses the provided floor_sqrt utility. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic without overflow. */
    seq_t xn = (seq_t)n;

    /* Compute r, the row number. */
    seq_t sq = (seq_t)8 * xn + (seq_t)1;            /* 8*n + 1 */
    seq_t s  = floor_sqrt(sq);                      /* floor(sqrt(8*n+1)) */
    seq_t r  = (s - (seq_t)1) / (seq_t)2;           /* floor((s-1)/2) */

    /* Compute the starting index of row r (offset). */
    seq_t start = r * (r + (seq_t)1) / (seq_t)2;    /* r*(r+1)/2 */

    /* Column within the row. */
    seq_t c = xn - start;

    /* Final triangle value. */
    seq_t value = (seq_t)3 * (r + c) + (seq_t)1;

    return value;
}
