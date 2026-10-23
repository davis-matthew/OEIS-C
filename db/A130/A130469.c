/*
 * A130469  Triangular array read by rows: T(j,k) = k*(j-k)! for k < j, T(j,k)
 *          = 1 for k = j; 1 <= k <= j.
 * https://oeis.org/A130469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(j,k) where j = smallest integer with j(j+1)/2 ≥ n, k =
 *            n – j(j–1)/2; a(n) = 1 if k == j else k·(j–k)!
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, but safe */

    /* Find the row index j */
    idx_t j = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    if ((j * (j + 1)) / 2 < n)
        ++j;

    idx_t t_prev = (j - 1) * j / 2;   /* triangular number up to row j-1 */
    idx_t k = n - t_prev;             /* column index within row j */

    if (k == j)
        return 1;                     /* last element in the row */

    /* Compute (j-k)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= j - k; ++i)
        fact *= (seq_t)i;

    return (seq_t)k * fact;           /* k * (j-k)! */
}
