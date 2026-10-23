/*
 * A135610  Triangle read by rows: the k-th entry of row n is the number of
 *          particular connectivity requirements that a k-linked graph with n
 *          >= 2k vertices has to satisfy T(n,k) = (1/2) * n!/(k!*(n-2*k)!)
 *          where k runs from 1 to floor(n/2).
 * https://oeis.org/A135610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 70, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A135610 sequence: triangle read by rows of
   T(n,k) = (1/2)*n!/(k!*(n-2k)!)  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r and position k such that the n-th term lies in row r,
       and k = position within that row (1‑based). */
    idx_t r = 2;                /* first row with entries */
    idx_t cumulative = 0;       /* number of entries before current row */

    while (1)
    {
        idx_t entries = r / 2;          /* floor(r/2) */
        if (cumulative + entries >= n) /* n lies in this row */
            break;
        cumulative += entries;
        r++;
    }

    idx_t k = n - cumulative;   /* position in the row (1 <= k <= floor(r/2)) */

    /* Compute T(r,k) = (1/2)*r!/(k!*(r-2k)!) using multiplicative binomials. */
    seq_t binom1 = 1;
    for (idx_t i = 1; i <= k; ++i)
        binom1 = binom1 * (seq_t)(r - i + 1) / (seq_t)i;          /* binom(r,k) */

    idx_t m = r - k;            /* m = r - k  (argument for binom(m,k)) */
    seq_t binom2 = 1;
    for (idx_t i = 1; i <= k; ++i)
        binom2 = binom2 * (seq_t)(m - i + 1) / (seq_t)i;          /* binom(m,k) */

    seq_t factk = 1;
    for (idx_t i = 2; i <= k; ++i)
        factk *= (seq_t)i;     /* k! */

    seq_t result = binom1 * binom2 * factk;
    result /= 2;              /* final division by 2 */

    return result;
}
