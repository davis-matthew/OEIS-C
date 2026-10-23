/*
 * A010236  Continued fraction for sqrt(194).
 * https://oeis.org/A010236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=13, a(n)=1 if n odd, a(n)=12 if n=2*odd, a(n)=26 if n
 *            divisible by 4
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)13;          /* offset term */
    /* Trailing zero count gives  v₂(n) */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0ULL) {
        e++;
        m >>= 1;
    }
    if (e == 0)        /* odd */
        return (seq_t)1;
    else if (e == 1)   /* 2 * odd */
        return (seq_t)12;
    else               /* 4 or more powers of 2 */
        return (seq_t)26;
}
