/*
 * A141433  Triangle T(n, k) = (k-1)*(3*n-k), read by rows.
 * https://oeis.org/A141433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) = (k-1)*(3*r-k) where r = smallest integer with
 *            r(r+1)/2 ≥ n and k = n - r(r-1)/2 (row-major flatten of the
 *            triangle T(n,k) = (k-1)*(3*n-k))
 * Range:     u8 n <= 77, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* determine the row r such that r(r+1)/2 >= n */
    idx_t low  = 1;
    idx_t high = 1;
    while (1) {
        __uint128_t val = (__uint128_t)high * (high + 1) / 2;
        if (val >= n) break;
        high <<= 1;           /* double until we exceed n */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* column index k */
    idx_t preceding = ( (__uint128_t)r * (r - 1) ) / 2;  /* sum of rows before r */
    idx_t k = n - preceding;                           /* 1‑based column within the row */

    /* compute T(r,k) */
    seq_t term = ((seq_t)k - 1) * ((seq_t)3 * r - k);
    return term;
}
