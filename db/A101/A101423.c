/*
 * A101423  Number of different cuboids with volume p^3 * q^n, where p,q are
 *          distinct prime numbers.
 * https://oeis.org/A101423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*(n+2)*(n+1) + 6*(floor(n/2)+1) + (n%3==0?2:0)) / 6
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* external linkage function required by the test harness */
seq_t a(idx_t n)
{
    /* work in seq_t to avoid overflows on 64‑bit builds */
    seq_t sn = (seq_t)n;

    /* (n+2)*(n+1)  */
    seq_t t = (sn + 2) * (sn + 1);

    /* 5 * t                         */
    seq_t A = 5 * t;

    /* floor(n/2) + 1                */
    seq_t B = (sn / 2) + 1;

    /* combine terms, adding 2 if n is a multiple of 3 */
    seq_t term = A + 6 * B;
    if ((n % 3) == 0) {
        term += 2;
    }

    /* final division by 6 */
    return term / 6;
}
