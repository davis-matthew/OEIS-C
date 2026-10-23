/*
 * A108732  a(0)=22; if n odd, a(n) = a(n-1)/2, otherwise a(n) = 4*a(n-1).
 * https://oeis.org/A108732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 55, u64 n <= 119, u128 n <= 247,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A108732
 * Recurrence:  a(0) = 22;  if n is odd  a(n) = a(n-1)/2,
 *                else           a(n) = 4*a(n-1).
 * Closed form: a(2k)   = 22 << k
 *              a(2k+1) = 11 << k      (k >= 0)
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For even n: a = 22 * 2^(n/2).  For odd n: a = 11 * 2^((n-1)/2). */
    if ((n & 1) == 0) {
        return ((seq_t)22) << (n / 2);
    } else {
        return ((seq_t)11) << ((n - 1) / 2);
    }
}
