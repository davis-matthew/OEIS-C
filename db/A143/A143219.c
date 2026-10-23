/*
 * A143219  Triangle read by rows, A127648 * A000012 * A127773, 1 <= k <= n.
 * https://oeis.org/A143219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row * col * (col + 1) / 2 where row is the smallest
 *            integer with row*(row+1)/2 >= n, and col = n - row*(row-1)/2
 * Range:     u8 n <= 35, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"   /* int_sqrt(idx_t n) */

/* Computes the n-th term of OEIS A143219. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range, but safe */

    /* Estimate the row using the quadratic formula. */
    idx_t r = (idx_t)((int_sqrt(8*n + 1) - 1) / 2);
    if ((r * (r + 1)) / 2 < n) r++;   /* adjust if needed */

    idx_t prev = (r * (r - 1)) / 2;   /* number of entries before this row   */
    idx_t c    = n - prev;            /* column index within the row          */

    /* Compute T(r, c) = r * c * (c + 1) / 2 */
    seq_t term = (seq_t)r *
                 ((seq_t)c * ((seq_t)c + 1) / 2);

    return term;
}
