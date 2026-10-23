/*
 * A014881  a(1)=1, a(n) = 11*a(n-1) + n.
 * https://oeis.org/A014881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + n, a(1) = 1 (OEIS A014881)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset 1, but return 0 for safety */
        return (seq_t)0;
    }

    seq_t value = (seq_t)1;                 /* a(1) */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        value = value * (seq_t)11 + (seq_t)i;   /* a(i) = 11*a(i-1) + i */
    }
    return value;
}
