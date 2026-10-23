/*
 * A164703  a(n) = 8*a(n-2) for n > 2; a(1) = 1, a(2) = 16.
 * https://oeis.org/A164703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) for n>2 with a(1)=1, a(2)=16
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)16;

    if (n & 1U) {               /* odd n: a(2k-1) = 8^(k-1) = 2^(3(k-1)) */
        idx_t k = (n - 1) / 2;   /* k = (n-1)/2 */
        return pow2(3 * k);
    } else {                    /* even n: a(2k) = 16*8^(k-1) = 2^(3k+1) */
        idx_t k = n / 2;        /* k = n/2 */
        return pow2(3 * k + 1);
    }
}
