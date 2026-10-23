/*
 * A101493  Triangle read by rows: T(n,k) = (n+1)*(2*(n+1)-1) - k*(2*k-1).
 * https://oeis.org/A101493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ((r+1)*(2*(r+1)-1)) – k*(2*k-1) where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) k = n – r*(r+1)/2 (column
 *            index) and T(r,k) = (r+1)*(2*(r+1)-1) – k*(2*k-1)
 * Range:     u8 n <= 65, u16 n <= 9804, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t r = 0;
    /* find row r such that n lies in that row */
    while (((r + 1) * (r + 2) / 2) <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    seq_t rr1 = (seq_t)(r + 1);
    seq_t kk  = (seq_t)k;
    seq_t val = rr1 * ((seq_t)(2 * rr1 - 1)) - kk * ((seq_t)(2 * kk - 1));
    return val;
}
