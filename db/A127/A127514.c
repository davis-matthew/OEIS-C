/*
 * A127514  Binomial transform of an infinite lower triangular matrix with
 *          mu(n) in the diagonal.
 * https://oeis.org/A127514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = binomial(r-1, c-1) * mu(c) where (r, c) are the row and
 *            column positions (1‑based) of n in the triangular indexing 1, 2
 *            3, 4 5 6, …
 * Range:     i8 n <= 40, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   binomial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C11 / C++17 compatible, uses provided utility functions binomial()
   and mobius(). */

#include <stdint.h>
#include <stddef.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/mobius.h"     /* seq_t mobius(idx_t m); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* harmless guard, undefined for n<1 */

    /* --- Find row r such that r*(r-1)/2 < n ≤ r*(r+1)/2 --- */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index c (1‑based) in that row */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_tri;   /* 1 ≤ c ≤ r */

    /* Compute the required binomial coefficient and Möbius value */
    seq_t bin = binomial(r - 1, c - 1);
    seq_t mu  = mobius(c);           /* mu(c) ∈ {−1,0,1} */

    return bin * mu;
}
