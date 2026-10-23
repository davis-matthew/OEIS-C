/*
 * A104764  Triangle T(n,k) = Lucas(n-k+1) read by rows, 1<=k<=n.
 * https://oeis.org/A104764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = Lucas(n-k+1) (read by rows) The sequence a(n) is the
 *            flat list of the triangle, so a(1) = T(1,1), a(2) = T(2,1), a(3)
 *            = T(2,2), a(4) = T(3,1), …
 * Range:     u8 n <= 66, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    /* map the flat index n to a row r and column k  (1‑based)  */
    idx_t r = 0, t = 0;
    while (t < n) {          /* find smallest r such that r(r+1)/2 >= n */
        r++;
        t += r;
    }
    idx_t prev_sum = (r - 1) * r / 2;      /* sum of 1…(r‑1)  */
    idx_t k = n - prev_sum;                /* column in row r   */
    idx_t idx = r - k + 1;                 /* argument for Lucas */

    return lucas(idx);
}
