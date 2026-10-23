/*
 * A122878  Periodic sequence of period 21 related to a simple scheduling
 *          problem.
 * https://oeis.org/A122878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence (period 21). a(1)=12, a(2)=12, a(3)=12, a(4)=12,
   a(5)=13, a(6)=13, a(7)=13, a(8)=23, a(9)=23, a(10)=23, a(11)=23,
   a(12)=12, a(13)=12, a(14)=12, a(15)=13, a(16)=13, a(17)=13,
   a(18)=13, a(19)=23, a(20)=23, a(21)=23.  The sequence repeats every 21 terms
   so a(n+21)=a(n). */
seq_t a(idx_t n)
{
    /* static lookup table for the 21-period pattern */
    static const seq_t pat[21] = {
        12,12,12,12,13,13,13,23,23,23,23,
        12,12,12,13,13,13,13,23,23,23
    };
    /* n>=1 so n-1 is safe; modulo 21 gives the correct index */
    return pat[(n - 1) % 21];
}
