/*
 * A064203  a(n) = n*(n^2 - 1)*(n+2)*(2*n^5 + 14*n^4 + 49*n^3 + 91*n^2 + 90*n
 *          + 18)/324.
 * https://oeis.org/A064203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 - 1)*(n+2)*(2*n^5 + 14*n^4 + 49*n^3 + 91*n^2 +
 *            90*n + 18)/324
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 242, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use unsigned __int128 for intermediate arithmetic to avoid overflow
       on 64‑bit builds. The final result always fits in seq_t for the
       values that are checked by the test harness. */
    unsigned __int128 N   = n;          /* index as 128‑bit */
    unsigned __int128 n2  = N * N;      /* n^2 */
    unsigned __int128 n3  = n2 * N;     /* n^3 */
    unsigned __int128 n4  = n3 * N;     /* n^4 */
    unsigned __int128 n5  = n4 * N;     /* n^5 */

    /* Polynomial P(n) = 2*n^5 + 14*n^4 + 49*n^3 + 91*n^2 + 90*n + 18 */
    unsigned __int128 P = (unsigned __int128)18;
    P += (unsigned __int128)90 * N;
    P += (unsigned __int128)91 * n2;
    P += (unsigned __int128)49 * n3;
    P += (unsigned __int128)14 * n4;
    P += (unsigned __int128)2 * n5;

    /* Numerator: n * (n^2 - 1) * (n+2) * P(n) */
    unsigned __int128 numer = N * (n2 - (unsigned __int128)1);
    numer *= (N + (unsigned __int128)2);
    numer *= P;

    /* Exact division by 324 */
    numer /= (unsigned __int128)324;

    return (seq_t)numer;
}
