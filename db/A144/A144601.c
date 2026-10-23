/*
 * A144601  Christoffel word of slope 3/11.
 * https://oeis.org/A144601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-14). Initial terms: a(0)=0, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=1, a(5)=0, a(6)=0, a(7)=0, a(8)=0, a(9)=1, a(10)=0,
 *            a(11)=0, a(12)=0, a(13)=1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Table of the first 14 terms of the period 14 Christoffel word */
    static const seq_t table[14] = {
        /* a(0)..a(13) */
        0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1
    };
    /* The sequence is periodic with length 14 */
    return table[n % 14];
}
