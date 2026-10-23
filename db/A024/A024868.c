/*
 * A024868  a(n) = 2*(n+1) + 3*n + ... + (k+1)*(n+2-k), where k = floor(n/2).
 * https://oeis.org/A024868
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)*(-2*floor(n/2)^2 + 3*n*floor(n/2) + 9*n +
 *            14)/6; (n ≥ 2)
 * Range:     u8 n <= 11, u16 n <= 89, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2: values for n < 2 are irrelevant */

    uint64_t n64 = (uint64_t)n;    /* safe for any n that fits in idx_t */
    uint64_t k   = n64 >> 1;        /* floor(n/2) */

    __int128 k128 = ( __int128 )k;
    __int128 inner = -2 * k128 * k128          /* -2k²   */
                     + 3 * ( __int128 )n64 * k128 /* 3nk   */
                     + 9 * ( __int128 )n64        /* 9n    */
                     + 14;                       /* 14    */

    __int128 res = k128 * inner / 6;          /* the formula */
    return (seq_t)res;                        /* cast to result type */
}
