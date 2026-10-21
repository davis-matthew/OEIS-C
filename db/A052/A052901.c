/*
 * A052901  Periodic with period 3: a(3n)=3, a(3n+1)=a(3n+2)=2.
 * https://oeis.org/A052901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3n) = 3, a(3n+1) = 2, a(3n+2) = 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 3.  idx_t is unsigned, so the result is nonnegative. */
    idx_t r = n % 3;
    return (r == 0) ? (seq_t)3 : (seq_t)2;
}
