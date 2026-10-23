/*
 * A036495  Nearest integer to n^(11/2).
 * https://oeis.org/A036495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n^(11/2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is: a(n) = round( sqrt( n^11 ) )                 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute N = n^11  (large integer) */
    seq_t N = big_pow((seq_t)n, (seq_t)11);

    /* s = floor( sqrt(N) )  (nearest integer not exceeding the exact value) */
    seq_t s = floor_sqrt(N);

    /* Distance from N to the lower square s^2 */
    seq_t sq = s * s;
    seq_t d = N - sq;

    /* Distance from N to the next square (s+1)^2 equals 2*s + 1 - d */
    seq_t two_s_plus1 = (s << 1) + 1;
    seq_t diff_upper = two_s_plus1 - d;

    /* Since a tie cannot occur, choose the nearer integer */
    if (d < diff_upper)
        return s;      /* lower integer is closer */
    else
        return s + 1;  /* upper integer is closer */
}
