/*
 * A107972  Triangle read by rows: T(n,k) = (k+1)(k+2)(n+2)(3n-2k+3)/12 for
 *          0<=k<=n.
 * https://oeis.org/A107972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = (k+1)(k+2)(r+2)(3r-2k+3)/12 where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute T(r,k) = (k+1)(k+2)(r+2)(3r-2k+3)/12 */
    seq_t term =
        ((seq_t)(k + 1) *
         (seq_t)(k + 2) *
         (seq_t)(r + 2) *
         (seq_t)(3 * r - 2 * k + 3)) /
        (seq_t)12;

    return term;
}
