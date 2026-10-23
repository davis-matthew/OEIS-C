/*
 * A132742  Triangle T(n,m) = 1 + ((2*n*3^m) mod 12), read by rows.
 * https://oeis.org/A132742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = 1 + ((2*r*3^c) mod 12), where r =
 *            floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2. This follows the
 *            definition T(n,m)=1+((2*n*3^m) mod 12), read by rows
 * Range:     u8 n <= 9703, u16 n <= 9703, u32 n <= 9703, u64 n <= 9703,
 *            u128 n <= 9703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Estimate the row number r from the quadratic formula. */
    long double d = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct r in case of floating‑point rounding errors. */
    while ((__int128)r * (r + 1) / 2 > n)
        --r;
    while ((__int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row. */
    idx_t m = n - (idx_t)((__int128)r * (r + 1) / 2);

    /* 3^m mod 12: 3^0 = 1, for m>=1 it cycles 3,9. */
    seq_t pow3 = (m == 0) ? 1 : ((m & 1) ? 3 : 9);

    /* (2*r*3^m) mod 12 */
    seq_t r_mod12      = r % 12;
    seq_t two_r_mod12  = (2 * r_mod12) % 12;
    seq_t mul          = (two_r_mod12 * pow3) % 12;

    return 1 + mul;
}
