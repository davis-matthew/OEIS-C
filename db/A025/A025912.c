/*
 * A025912  Expansion of 1/((1-x^7)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A025912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n^2+2*n+3)/20) + floor((2*n^2+7*n)/9) -
 *            floor((4*n^2+6*n-3)/7)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A025912: partitions of n into parts 7, 9, and 10 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a signed 128‑bit integer for intermediate arithmetic
       to avoid overflow for reasonably large n (e.g. up to 10^10).   */
    __int128 nn = ( __int128 ) n;

    __int128 t1 = (7*nn*nn + 2*nn + 3) / 20;
    __int128 t2 = (2*nn*nn + 7*nn)        / 9;

    __int128 num3 = 4*nn*nn + 6*nn - 3;
    __int128 t3   = num3 / 7;          /* truncates toward zero in C / C++ */

    /* Adjust for floor division when num3 is negative */
    if (num3 < 0 && num3 % 7 != 0) {
        t3 -= 1;
    }

    return (seq_t)(t1 + t2 - t3);
}
