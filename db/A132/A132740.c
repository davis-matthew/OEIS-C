/*
 * A132740  Largest divisor of n coprime to 10.
 * https://oeis.org/A132740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = largest divisor of n coprime to 10 (i.e., remove all
 *            factors of 2 and 5 from n)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    idx_t tmp = n;
    /* Remove all factors of 2 */
    while ((tmp & 1ULL) == 0)
        tmp >>= 1;
    /* Remove all factors of 5 */
    while (tmp % 5 == 0)
        tmp /= 5;
    return (seq_t)tmp;
}
