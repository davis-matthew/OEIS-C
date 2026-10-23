/*
 * A156991  Triangle T(n,k) read by rows: T(n,k) = n! * binomial(n + k - 1,
 *          n).
 * https://oeis.org/A156991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = product_{i=0}^{r-1} (k+i), where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 53, u64 n <= 132,
 *            u128 n <= 348, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A156991. */
seq_t a(idx_t n)
{
    /* Find row r and column k such that n is the (k)-th entry in row r.
       The number of entries before row r is r*(r+1)/2. */
    idx_t r = 0;
    idx_t tri = 0;      /* triangular number r*(r+1)/2 */

    /* Increment r until the next triangular number exceeds n. */
    while (tri + r + 1 <= n) {
        r++;
        tri += r;
    }

    idx_t k = n - tri;     /* column index within the row */

    /* Compute T(r,k) = product_{i=0}^{r-1} (k+i) for r>0.
       For r==0, T(0,0)=1. */
    if (r == 0)
        return (seq_t)1;

    seq_t result = 1;
    for (idx_t i = 0; i < r; ++i)
        result *= (seq_t)(k + i);

    return result;
}
