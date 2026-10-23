/*
 * A144151  Triangle T(n,k), n>=0, 0<=k<=n, read by rows: T(n,k) = number of
 *          ways an undirected cycle of length k can be built from n labeled
 *          nodes.
 * https://oeis.org/A144151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = C(n,k) if k<=2, else T(n,k) = C(n,k)*(k-1)!/2. a(n) =
 *            T(r,k) where r is the row such that n < (r+1)(r+2)/2 and k = n -
 *            r(r+1)/2
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 131, u64 n <= 273,
 *            u128 n <= 698, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r for index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;

    /* Column k within that row. */
    idx_t k = n - r * (r + 1) / 2;

    seq_t bin = binomial(r, k);

    if (k <= 2)
        return bin;                    /* C(n,k) when k<=2 */

    /* For k>=3: T = C(n,k)*(k-1)!/2.
       Compute (k-1)! and divide by 2 first to keep intermediate small. */
    seq_t fact = factorial(k - 1);
    fact /= 2;                          /* exact division because (k-1)! is even for k>=3 */

    return fact * bin;                  /* exact product in seq_t */
}
