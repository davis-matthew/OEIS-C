/*
 * A049828  Triangular array T given by rows: T(n,k)=sum of remainders when
 *          Euclidean algorithm acts on n,k; for k=1,2,...,n; n >= 1.
 * https://oeis.org/A049828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangular array T(n,k)
 *            (row‑wise printed). That is, a(n) = T(r, k) where r satisfies
 *            (r-1)*r/2 < n ≤ r*(r+1)/2 and k = n - (r-1)*r/2 T(r,k) is the
 *            sum of the remainders produced by the Euclidean algorithm
 *            applied to the pair (r, k)
 * Range:     u8 n <= 8859, u16 n <= 8859, u32 n <= 8859, u64 n <= 8859,
 *            u128 n <= 8859, big any n
 * Helpers:   sum_remainders
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_remainders.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 – return 0 for safety */
        return 0;

    /* find row r such that (r-1)*r/2 < n <= r*(r+1)/2 */
    idx_t r = 1;
    idx_t tri = 1;            /* r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;             /* add next row length */
    }

    /* column k = n - (r-1)*r/2 */
    idx_t prev_tri = tri - r; /* (r-1)*r/2 */
    idx_t k = n - prev_tri;

    return sum_remainders(r, k);
}
