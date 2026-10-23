/*
 * A127013  Triangle read by rows: row reversal of A126988.
 * https://oeis.org/A127013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r/(r-p+1) if (r mod (r-p+1)) == 0, else 0. where r is the
 *            row containing the n'th term: r(r+1)/2 ≥ n, and p = n – r(r‑1)/2
 *            is the position within that row (1‑based)
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* find the row number r such that r*(r+1)/2 ≥ n */
    idx_t D = 1 + 8*n;
    idx_t s = int_sqrt(D);              /* floor sqrt */
    if (s*s < D) s++;                    /* ceil sqrt */
    idx_t r = (s - 1) / 2;              /* floor of (ceil√D-1)/2 */
    if (r*(r+1)/2 < n) r++;              /* adjust to satisfy inequality */

    /* position of the term within the row (1‑based) */
    idx_t prev = r*(r-1)/2;
    idx_t p = n - prev;

    /* j is the reversed index: j = r - p + 1 */
    idx_t j = r - p + 1;
    if (j == 0) return (seq_t)0;        /* defensive, should not happen */

    /* compute value: r/j if divisible, else 0 */
    if (r % j == 0)
        return (seq_t)(r/j);
    else
        return (seq_t)0;
}
