/*
 * A152890  Periodic sequence [4,1,4,0,1] of period 5.
 * https://oeis.org/A152890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence [4,1,4,0,1] of period 5.
   a(0)=4, a(1)=1, a(2)=4, a(3)=0, a(4)=1,
   a(n+5)=a(n). */
seq_t a(idx_t n)
{
    static const seq_t period[5] = {4,1,4,0,1};
    return period[n % 5];
}
