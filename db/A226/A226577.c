/*
 * A226577  Smallest number of integer-sided squares needed to tile a 4 X n
 *          rectangle.
 * https://oeis.org/A226577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/4) + (n%4==0?0: (n%4==2?2:4))
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1 + a(n-4) for n>3 with a(0)=0, a(1)=4, a(2)=2, a(3)=4 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t div4 = (seq_t)n / 4;      /* floor(n/4) computed in seq_t */
    idx_t r = n % 4;                /* remainder modulo 4 */

    static const seq_t offset[4] = {0, 4, 2, 4};
    return div4 + offset[r];
}
