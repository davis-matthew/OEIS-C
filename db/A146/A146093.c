/*
 * A146093  Bell numbers (A000110) read mod 3.
 * https://oeis.org/A146093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: Bell numbers (A000110) read modulo 3 have period 13:
   a(n+13) = a(n). */

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 2, a(4) = 0,
       a(5) = 1, a(6) = 2, a(7) = 1, a(8) = 0, a(9) = 0,
       a(10) = 1, a(11) = 0, a(12) = 1 */
    static const seq_t tbl[13] = {1,1,2,2,0,1,2,1,0,0,1,0,1};
    return tbl[n % 13ULL];
}
