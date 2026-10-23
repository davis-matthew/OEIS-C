/*
 * A101676  a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) with initial
 *          terms 1,0,-2,-1,0.
 * https://oeis.org/A101676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5), with initial
 *            terms a(0)=1, a(1)=0, a(2)=-2, a(3)=-1, a(4)=0, a(5)=0. The
 *            sequence has period 6, repeating 1,0,-2,-1,0,0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of length 6 */
    static const seq_t vals[6] = {1, 0, -2, -1, 0, 0};
    return vals[n % 6];
}
