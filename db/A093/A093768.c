/*
 * A093768  Positive first differences of the rows of triangle A088459, which
 *          enumerates symmetric Dyck paths.
 * https://oeis.org/A093768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 73, u32 n <= 223, u64 n <= 696,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093768: a(n) is the n‑th flattened element of triangle A088459.
   Triangle entry
        T(n,k) = binomial(n+1, ceil(k/2)) * binomial(n, floor(k/2))
                - binomial(n+1, ceil((k‑1)/2)) * binomial(n, floor((k‑1)/2))
   for 0 ≤ k ≤ n.  The sequence is obtained by reading the triangle row‑by‑row.   */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial.h"

static seq_t triangle_T(idx_t r, idx_t k)
{
    if (k > r) return (seq_t)0;

    /* first term */
    idx_t c1 = (k + 1) / 2;          /* ceil(k/2)   */
    idx_t c2 = k / 2;                /* floor(k/2)  */
    seq_t term1 = binomial(r + 1, c1) * binomial(r, c2);

    if (k == 0) return term1;

    /* second term */
    idx_t d1 = k / 2;                /* ceil((k-1)/2) */
    idx_t d2 = (k - 1) / 2;          /* floor((k-1)/2) */
    seq_t term2 = binomial(r + 1, d1) * binomial(r, d2);

    return term1 - term2;
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - start;             /* position in the row */

    return triangle_T(r, k);
}
