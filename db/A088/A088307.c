/*
 * A088307  Triangle, read by rows, T(n,k) = n^2 + k^2 if gcd(n,k)=1,
 *          otherwise 0.
 * https://oeis.org/A088307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r-1)/2, and T(r,k) = r^2 + k^2 if gcd(r,k) = 1, else 0
 * Range:     u8 n <= 76, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   gcd_idx_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function is written to compile in both C11 and C++17. */

#include <stdint.h>
#include "../utils/gcd_idx_local.h"

seq_t a(idx_t n)
{
    /* Find the triangle row r and column k corresponding to index n */
    idx_t r = 1;
    idx_t cumulative = 1;            /* number of entries up to current row r */

    while (cumulative < n) {
        ++r;
        cumulative += r;             /* cumulative now includes the new row */
    }

    idx_t k = n - (cumulative - r); /* column in row r */

    /* Compute the value T(r,k) only if r and k are coprime */
    if (gcd_idx_local(r, k) != 1)
        return 0;

    /* Use seq_t for the arithmetic to avoid overflow. */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    return rr * rr + kk * kk;
}
