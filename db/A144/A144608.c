/*
 * A144608  Christoffel word of slope 10/11.
 * https://oeis.org/A144608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-21)
   Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=1, a(5)=0,
   a(6)=1, a(7)=0, a(8)=1, a(9)=0, a(10)=1, a(11)=0, a(12)=1,
   a(13)=0, a(14)=1, a(15)=0, a(16)=1, a(17)=0, a(18)=1, a(19)=0,
   a(20)=1
   Thus the sequence is periodic with period 21. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 21-element period table */
    static const seq_t table[21] = {
        0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1
    };
    idx_t r = n % 21;
    return table[(size_t)r];
}
