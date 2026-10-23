/*
 * A040807  Continued fraction for sqrt(836).
 * https://oeis.org/A040807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=28; for n>0, a(n)=a(n-10) with initial values a(1)=1,
 *            a(2)=10, a(3)=1, a(4)=1, a(5)=2, a(6)=1, a(7)=1, a(8)=10,
 *            a(9)=1, a(10)=56
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage required */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)28;
    }
    /* periodic part: 1,10,1,1,2,1,1,10,1,56 */
    static const seq_t periodic[10] = {
        (seq_t)1, (seq_t)10, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)10,
        (seq_t)1, (seq_t)56
    };
    idx_t m = (n - 1) % 10;
    return periodic[m];
}
