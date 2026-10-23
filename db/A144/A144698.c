/*
 * A144698  Triangle of 4-Eulerian numbers.
 * https://oeis.org/A144698
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(n, n‑4) * where T is the triangle of 4‑Eulerian numbers
 *            defined by * T(n,0)=1 (n≥4) * T(4,k)=0 for k≥1 * T(n,k) =
 *            (k+1)·T(n‑1,k) + (n‑k)·T(n‑1,k‑1) (n≥5, 0≤k≤n‑4) * * The routine
 *            walks the triangle row by row, keeping only two * consecutive
 *            rows in memory so that a(n) can be obtained in...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 74, u64 n <= 201,
 *            u128 n <= 545, big any n
 * Helpers:   tri_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/tri_value.h"

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;          /* undefined for n<4 */

    /* Convert the 1‑based index n into a 0‑based position m within the
       triangle, then locate the corresponding row r and column k. */
    idx_t m = n - 4;                     /* 0‑based offset */
    idx_t r = 4;                         /* current row number   */
    idx_t cumulative = 0;                /* elements before row r */

    while (cumulative + (r - 3) <= m) {
        cumulative += (r - 3);
        ++r;
    }

    idx_t k = m - cumulative;            /* column within the row r */

    return tri_value(r, k);
}
