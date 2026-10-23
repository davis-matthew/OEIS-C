/*
 * A046163  Reduced denominators of (n-1)^2/(n^2 + n + 1).
 * https://oeis.org/A046163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n + 1) / ( (n % 3 == 1) ? 3 : 1 )
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reduced denominators of (n-1)^2/(n^2 + n + 1) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* behavior for n < 1 is irrelevant */
        return 0;

    /* use seq_t for all arithmetic to preserve arbitrary precision */
    seq_t nn = (seq_t)n;
    seq_t denom = nn * nn + nn + 1;

    /* divide by 3 precisely when n ≡ 1 (mod 3) */
    if ((n % 3) == 1)
        denom /= (seq_t)3;

    return denom;
}
