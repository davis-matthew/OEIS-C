/*
 * A157654  Triangle T(n, k, m) = 1 if k = 0 or k = n, otherwise m*abs(
 *          (n-k)^(m-1) - k^(m-1) ), with m = 2, read by rows.
 * https://oeis.org/A157654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = floor((sqrt(8*n+1)-1)/2) k = n -
 *            r*(r+1)/2 and T(r,k)=1 if k=0 or k=r else 2*abs(r-2*k)
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t t;
    while (1) {
        t = r * (r + 1) / 2;
        if (t > n) break;
        /* if next row would exceed n, break */
        idx_t next = (r + 1) * (r + 2) / 2;
        if (next > n) break;
        ++r;
    }
    /* Compute column k in that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r,k) */
    if (k == 0 || k == r) {
        return (seq_t)1;
    } else {
        /* abs(r-2*k) */
        idx_t diff = r > 2 * k ? r - 2 * k : 2 * k - r;
        return (seq_t)2 * (seq_t)diff;
    }
}
