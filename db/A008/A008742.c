/*
 * A008742  Molien series for 3-dimensional group [3,3 ]+ = 332.
 * https://oeis.org/A008742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2+3*n+20)/24 + (2*n+3)*(-1)^n/16)
 * Range:     u8 n <= 75, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n >= 0.
 * Uses a closed‐form expression derived from the OEIS formula.
 * Uses signed __int128 for intermediate arithmetic to avoid overflow
 * for reasonably large n (still well below 2^63). */
seq_t a(idx_t n)
{
    /* Cast n to __int128 for safe intermediate calculations. */
    signed __int128 nn = (signed __int128)n;        /* n  */
    signed __int128 n2 = nn * nn;                  /* n^2 */
    /* Core numerator:
     *  2*(n^2 + 3*n + 20)  +  (-1)^n * 3*(2*n + 3)
     */
    signed __int128 numerator = 2 * (n2 + 3 * nn + 20);
    signed __int128 two_n_plus3 = 2 * nn + 3;
    if ((n & 1u) == 0)                          /* even n: (+) */
        numerator += 3 * two_n_plus3;
    else                                         /* odd n: (-) */
        numerator -= 3 * two_n_plus3;

    /* Final result: floor(numerator / 48) */
    seq_t result = (seq_t)(numerator / 48);
    return result;
}
