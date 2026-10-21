/*
 * A099567  Riordan array (1/(1-x-x^3), 1/(1-x)).
 * https://oeis.org/A099567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row, col) where the Riordan triangle has T(n,k) =
 *            ∑_{j=0..⌊n/3⌋} binomial(n-2j, k+j). The sequence enumerates the
 *            triangle row‑wise: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1),
 *            a(3)=T(2,0), …
 * Range:     u8 n <= 39, u16 n <= 163, u32 n <= 643, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>                     /* for sqrt() to locate the row */
#include "../utils/binomial.h"        /* provides seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* Number of elements before this row (triangular number).  Use 128‑bit to avoid overflow. */
    idx_t before = (idx_t)(((__int128)r * (r + 1)) / 2);

    idx_t col = n - before;          /* column within the row */

    seq_t total = 0;
    for (idx_t j = 0; j <= r / 3; ++j) {
        idx_t a_n = r - 2 * j;       /* top index in binomial */
        idx_t a_k = col + j;         /* lower index in binomial */
        if (a_k > a_n)               /* binomial() returns 0 for k>n */
            continue;
        total += binomial(a_n, a_k);
    }
    return total;
}
