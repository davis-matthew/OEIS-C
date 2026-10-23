/*
 * A159623  Triangle read by rows: T(n, k) = n!*q^k/(n-k)! if floor(n/2) > k-1
 *          otherwise n!*q^(n-k)/k!, with q = 1.
 * https://oeis.org/A159623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) (row index),
 *            * k = n - r*(r+1)/2 (column index), * T(n,k) = n!/(n-k)! if k <=
 *            floor(n/2) else n!/k! (with q = 1)
 * Range:     u8 n <= 38, u16 n <= 82, u32 n <= 179, u64 n <= 479,
 *            u128 n <= 1299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + r + 1 <= n) {
        cum += r + 1;
        r++;
    }

    /* Position within the row */
    idx_t k = n - cum;

    /* Compute the triangle entry */
    seq_t result = 1;
    if (k <= r / 2) {
        /* Use falling factorial: n!/(n-k)! = product_{i=n-k+1}^{n} i */
        for (idx_t i = r - k + 1; i <= r; ++i) {
            result *= (seq_t)i;
        }
    } else {
        /* Use n!/k! = product_{i=k+1}^{n} i */
        for (idx_t i = k + 1; i <= r; ++i) {
            result *= (seq_t)i;
        }
    }
    return result;
}
