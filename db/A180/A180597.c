/*
 * A180597  Digital root of 7n.
 * https://oeis.org/A180597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digital root of 7*n Digital root of m is 0 if m==0,
 *            otherwise 1+((m-1)%9)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* 7*0 = 0, digital root = 0 */

    /* compute (7*n) modulo 9 without overflow */
    const idx_t mod = 9;
    idx_t r = (7 * (n % mod)) % mod;   /* r in 0..8 */

    /* if r==0, the digital root is 9 (since 7*n > 0) */
    return (r == 0) ? (seq_t)9 : (seq_t)r;
}
