/*
 * A079904  Triangle read by rows: T(n, k) = n*k, 0 <= k <= n.
 * https://oeis.org/A079904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Let r = floor((sqrt(8*n+1)-1)/2). Then a(n) = r * (n -
 *            r*(r+1)/2)
 * Range:     u8 n <= 7, u16 n <= 1655, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute the row number r */
    seq_t val = (seq_t)n;                 /* n as seq_t */
    seq_t t   = val * 8 + 1;             /* 8*n + 1 */
    seq_t s   = floor_sqrt(t);           /* floor(sqrt(8*n+1)) */
    seq_t r   = (s - 1) / 2;             /* floor((sqrt(8*n+1)-1)/2) */

    /* Compute the offset k within the row */
    seq_t start = r * (r + 1) / 2;       /* first index of row r */
    seq_t offset = val - start;          /* k = n - start */

    /* Result is n*k = r*k */
    seq_t result = r * offset;
    return result;
}
