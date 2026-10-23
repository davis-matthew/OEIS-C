/*
 * A165943  a(n) = A061037(7*n+2).
 * https://oeis.org/A165943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = -7*n*(7*n + 4)*(27*(-1)^n + 6*cos((Pi*n)/2) - 37)/64
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>

seq_t a(idx_t n)
{
    /* (-1)^n */
    int sgn = (n & 1) ? -1 : 1;

    /* cos(Pi*n/2) takes values 1,0,-1,0 periodically. */
    int cos_val;
    switch (n & 3) {
        case 0: cos_val =  1; break;      /* n ≡ 0 (mod 4) */
        case 2: cos_val = -1; break;      /* n ≡ 2 (mod 4) */
        default: cos_val =  0; break;     /* n ≡ 1 or 3 (mod 4) */
    }

    /* 7*n*(7*n + 4) */
    __int128 part1 = (__int128)7 * n * ( (__int128)7 * n + 4 );

    /* inner bracket: 27*(-1)^n + 6*cos(...) - 37 */
    __int128 inner = (__int128)27 * sgn + (__int128)6 * cos_val - 37;

    /* numerator: -part1 * inner */
    __int128 numer = -part1 * inner;

    /* division by 64 */
    __int128 denom = 64;
    __int128 val = numer / denom;

    return (seq_t)val;
}
