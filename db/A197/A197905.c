/*
 * A197905  Ceiling((n+1/n)^6).
 * https://oeis.org/A197905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n + 1/n)^6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work in unsigned __int128 to avoid overflow for n up to ~10^4.
       The result fits in seq_t for all values that fit in the
       target integer type. */
    __int128 nn = (__int128)n;

    __int128 n2 = nn * nn;           /* n^2      */
    __int128 n4 = n2 * n2;           /* n^4      */
    __int128 n6 = n4 * n2;           /* n^6      */

    /* Fractional part: (15/n^2 + 6/n^4 + 1/n^6) = (15*n^4 + 6*n^2 + 1)/n^6 */
    __int128 num = (__int128)15 * n4 + (__int128)6 * n2 + 1;  /* numerator */
    __int128 floor_part = num / n6;          /* integer part of fractional part */
    __int128 rem = num % n6;                 /* if non-zero, we need to add 1 */
    __int128 add = floor_part + (rem > 0 ? 1 : 0);

    /* Main integer terms: n^6 + 6*n^4 + 15*n^2 + 20 */
    __int128 res = n6 + (__int128)6 * n4 + (__int128)15 * n2 + 20 + add;

    return (seq_t)res;
}
