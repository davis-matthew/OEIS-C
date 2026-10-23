/*
 * A160743  8*P_7(n), 8 times the Legendre Polynomial of order 7 at n.
 * https://oeis.org/A160743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(429*n^6 - 693*n^4 + 315*n^2 - 35)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 11, u64 n <= 262, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    /* Use a signed 128‑bit type for intermediate calculations.
       This is large enough for n up to at least 10^4 (the
       limits of the test harness). */
    __int128 n128 = n;
    __int128 n2   = n128 * n128;    /* n^2 */
    __int128 n4   = n2 * n2;        /* n^4 */
    __int128 n6   = n4 * n2;        /* n^6 */

    __int128 term = (__int128)429 * n6
                    - (__int128)693 * n4
                    + (__int128)315 * n2
                    - (__int128)35;

    __int128 res  = n128 * term / 2; /* divide by 2 after multiplication */

    return (seq_t)res;
}
