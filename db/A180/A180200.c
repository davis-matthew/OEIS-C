/*
 * A180200  a(0)=0, a(1)=1; for n > 1, a(n) = 2*m + 1 - (n mod 2 + m mod 2)
 *          mod 2, where m = a(floor(n/2)).
 * https://oeis.org/A180200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0, a(1)=1; for n>1, a(n) = 2*a(n/2) + [n mod 2 == a(n/2)
 *            mod 2] (with 1 added when the parities are equal)
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n < 2U)
        return (seq_t)n;

    /* recursive computation on floor(n/2) */
    seq_t m = a(n >> 1U);            /* m = a(floor(n/2)) */

    seq_t res = 2 * m;               /* 2 * m */

    /* add 1 if n and m share the same parity */
    if ((n & 1U) == (m & 1))
        res += 1;

    return res;
}
