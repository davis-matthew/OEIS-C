/*
 * A162995  A scaled version of triangle A162990.
 * https://oeis.org/A162995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Let r = ceil((sqrt(8*n+1)-1)/2), m = n - r*(r-1)/2. Then a(n) =
 *            (r+1)!/(m+1)! = ∏_{k=m+2}^{r+1} k
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 78, u64 n <= 190,
 *            u128 n <= 561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Index of the term within its row (1‑based) */
    idx_t prev = (r * (r - 1)) / 2;   /* terms before this row */
    idx_t m = n - prev;               /* 1 <= m <= r */

    /* Compute the falling factorial (r+1)!/(m+1)! */
    seq_t val = 1;
    for (idx_t k = m + 2; k <= r + 1; ++k)
        val *= (seq_t)k;

    return val;
}
