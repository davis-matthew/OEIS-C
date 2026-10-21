/*
 * A088889  Polynexus numbers of order 8.
 * https://oeis.org/A088889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^8-(n-1)^8)-(n^4-(n-1)^4))/240
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 39, u64 n <= 919, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Use 128‑bit intermediate to avoid overflow when seq_t is 64‑bit. */
    __int128 nn  = (__int128)n;
    __int128 n1  = nn - 1;

    /* n^2, n^4, n^8 and (n-1)^2, (n-1)^4, (n-1)^8 */
    __int128 n2   = nn * nn;
    __int128 n4   = n2 * n2;
    __int128 n8   = n4 * n4;

    __int128 n12  = n1 * n1;
    __int128 n14  = n12 * n12;
    __int128 n18  = n14 * n14;

    /* compute numerator: ((n^8-(n-1)^8)-(n^4-(n-1)^4)) */
    __int128 numerator = (n8 - n18) - (n4 - n14);

    /* divide by 240 */
    __int128 result128 = numerator / 240;

    /* cast back to seq_t; fits if result fits seq_t */
    return (seq_t)result128;
}
