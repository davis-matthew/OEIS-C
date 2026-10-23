/*
 * A118921  Triangle read by rows: T(n,k) is the number of Grand Dyck paths of
 *          semilength n having first return to the x-axis at (2k,0) (n,k >=
 *          1). (A Grand Dyck path of semilength n is a path in the half-plane
 *          x >= 0, starting at (0,0), ending at (2n,0) and consisting of
 *          steps u=(1,1) and d=(1,-1)).
 * https://oeis.org/A118921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 >= n k =
 *            n - (r-1)r/2 T(r,k) = 2 * C(2k-2,k-1) * C(2r-2k,r-k) / k (C is
 *            binomial coefficient)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1 */

    /* Find the row r (1‑based) that contains position n */
    idx_t r = 1;
    while (((__int128)r * (r + 1)) / 2 < n) ++r;

    /* Compute column k within that row */
    idx_t prev = (r - 1) * r / 2;        /* entries before row r */
    idx_t k = n - prev;                  /* 1‑based column index */

    /* Binomial coefficients needed for the formula */
    seq_t b1 = binomial(2 * k - 2, k - 1);          /* C(2k-2, k-1) */
    seq_t b2 = binomial(2 * r - 2 * k, r - k);      /* C(2r-2k, r-k) */

    /* Combine using the formula */
    seq_t res = 2 * b1 * b2 / (seq_t)k;
    return res;
}
