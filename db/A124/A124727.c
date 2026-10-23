/*
 * A124727  Triangle read by rows: T(n,k)=k*binomial(n-1,k-1)+binomial(n-1,k)
 *          (1<=k<=n).
 * https://oeis.org/A124727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = k*binomial(r-1,k-1) +
 *            binomial(r-1,k), and r,k are obtained from the position n in the
 *            triangle read by rows
 * Range:     u8 n <= 39, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n and (r-1)r/2 < n */
    double d = (-1.0 + sqrt(1.0 + 8.0 * (double)n)) / 2.0;
    idx_t r = (idx_t)ceill(d);   /* ceil to ensure enough terms */
    /* position in the row (1‑based) */
    idx_t prev = (r - 1) * r / 2;             /* total terms before row r */
    idx_t k = n - prev;                       /* column index (1‑based) */

    /* Compute the binomial coefficients */
    seq_t c1 = binomial(r - 1, k - 1);        /* C(r-1, k-1) */
    seq_t c2 = binomial(r - 1, k);            /* C(r-1, k)   */

    /* Result: k*C(r-1, k-1) + C(r-1, k) */
    seq_t result = (seq_t)k * c1 + c2;
    return result;
}
