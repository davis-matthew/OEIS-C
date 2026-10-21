/*
 * A076221  Triangle read by rows: A(n,k) is the number of x, x<=n, which are
 *          coprime to and not equal to k.
 * https://oeis.org/A076221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = |{1 ≤ x ≤ r | gcd(x,k)=1, x≠k}| where * r = ceil( (-1 +
 *            sqrt(1+8n)) / 2 ) (row index) * k = n - r*(r-1)/2 (column index)
 *            * For the triangle read by rows of A(n,k)
 * Range:     u8 n <= 9723, u16 n <= 9723, u32 n <= 9723, u64 n <= 9723,
 *            u128 n <= 9723, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        r++;

    /* Index within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;           /* 1‑based column of the triangle */

    /* Count coprime numbers ≤ r that are not equal to k */
    seq_t cnt = 0;
    seq_t sk = (seq_t)k;

    for (idx_t x = 1; x <= r; ++x) {
        if (x == k)
            continue;
        seq_t sx = (seq_t)x;
        if (gcd(sx, sk) == (seq_t)1)
            ++cnt;
    }

    return cnt;
}
