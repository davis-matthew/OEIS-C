/*
 * A071286  Denominators of Peirce sequence of order 5.
 * https://oeis.org/A071286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A071286 – Denominators of Peirce sequence of order 5
 * The sequence is periodic with period 15:
 *   a(0)=2, a(1)=4, a(2)=5, a(3)=3, a(4)=1,
 *   a(5)=5, a(6)=4, a(7)=3, a(8)=5, a(9)=4,
 *   a(10)=2, a(11)=5, a(12)=3, a(13)=4, a(14)=5
 * and for n>14, a(n)=a(n-15).
 *
 * Formula: a(n) = period[n % 15]   where period = {2,4,5,3,1,5,4,3,5,4,2,5,3,4,5}
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* period of length 15 */
    static const seq_t period[15] = {
        (seq_t)2, (seq_t)4, (seq_t)5, (seq_t)3, (seq_t)1,
        (seq_t)5, (seq_t)4, (seq_t)3, (seq_t)5, (seq_t)4,
        (seq_t)2, (seq_t)5, (seq_t)3, (seq_t)4, (seq_t)5
    };

    /* n is unsigned, modulo 15 fits in idx_t */
    idx_t pos = n % 15U;
    return period[pos];
}
