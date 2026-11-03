/*
 * A092271  Triangle read by rows. First in a series of triangular arrays
 *          counting permutations of partitions.
 * https://oeis.org/A092271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A092271 : triangle read by rows
   a(n) = C(r, k-1) * (r-k)!   (r*(r+1)/2 >= n > (r-1)*r/2, k = n - (r-1)*r/2)
   a(r,r) = 1                                     
   Here r is the row number and k the column index (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains index n */
    idx_t r = 1;
    idx_t cum = 1;                  /* cumulative count up to row r              */
    while (cum < n) {
        ++r;
        cum += r;                   /* cum = r*(r+1)/2 after this addition          */
    }

    /* Positions before this row: (r-1)*r/2 = cum - r                */
    idx_t prev = cum - r;
    idx_t k = n - prev;             /* column index (1 … r)                        */

    if (k == r)                     /* last element of the row                     */
        return (seq_t)1;

    seq_t bin = binomial(r, k - 1);  /* C(r, k-1)                                   */
    seq_t fact = factorial(r - k);   /* (r-k)!                                      */

    return bin * fact;               /* C(r, k-1) * (r-k)!                          */
}
