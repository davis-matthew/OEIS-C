/*
 * A177263  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having k as the last entry in the first block
 *          (1<=k<=n).
 * https://oeis.org/A177263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   cumfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cumfact.h"
/* Triangle entry T(n,k) of A177263:
   if k < n:   T(n,k) = (n-1)! – (n-k-1)!
   if k == n:  T(n,n) = 0! + 1! + … + (n-1)!
   The sequence a(n) enumerates the triangle row‑by‑row:
   a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …   */

/* Compute factorial of a non‑negative integer m  */
static seq_t factorial(idx_t m)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i) {
        res *= (seq_t)i;
    }
    return res;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* out‑of‑range, but harmless */

    /* find the row r such that r*(r+1)/2 ≥ n  (row numbering starts at 1) */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* previous total elements before row r */
    idx_t prev = (r - 1) * r / 2;
    idx_t k    = n - prev;              /* column in this row, 1‑based */

    if (k < r) {                        /* first r‑1 columns  */
        seq_t f1 = factorial(r - 1);            /* (n-1)!  */
        seq_t f2 = factorial(r - k - 1);        /* (n-k-1)! */
        return f1 - f2;
    } else {                            /* last column (k==r)   */
        return cumfact(r);
    }
}
