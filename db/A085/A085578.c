/*
 * A085578  Array read by antidiagonals: T(n,m) is the number of distinct
 *          products i*j with 1<=i<=n, i<=j<=m, for n>=1, m>=1.
 * https://oeis.org/A085578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A085578
    a(n) = T(i,j) where T(i,j) is the number of distinct products i*j
    with 1 <= i <= n, 1 <= j <= m, read by antidiagonals:
    a(1) = T(1,1), a(2) = T(1,2), a(3) = T(2,1), a(4) = T(1,3), a(5) = T(2,2), a(6) = T(3,1), …
*/

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* locate the diagonal s such that s*(s-1)/2 >= n */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n)
        ++s;

    /* number of terms before this diagonal */
    idx_t prev = (s - 2) * (s - 1) / 2;
    /* position within the diagonal (0‑based) */
    idx_t pos  = n - prev - 1;

    /* coordinates on the diagonal: (i, j) = (pos+1, (s-1)-pos) */
    idx_t i = pos + 1;
    idx_t j = (s - 1) - pos;

    /* maximum product we need to index */
    idx_t max_prod = i * j;

    /* mark products that have already appeared */
    unsigned char *seen = (unsigned char *)calloc((size_t)max_prod + 1, 1);
    if (!seen)
        return 0;                     /* out of memory → return 0 as fallback */

    seq_t count = 0;
    for (idx_t x = 1; x <= i; ++x) {
        for (idx_t y = 1; y <= j; ++y) {
            idx_t prod = x * y;
            if (!seen[prod]) {
                seen[prod] = 1;
                ++count;
            }
        }
    }

    free(seen);
    return count;
}
