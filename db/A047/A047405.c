/*
 * A047405  Numbers that are congruent to {0, 1, 2, 3, 6} mod 8.
 * https://oeis.org/A047405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {0,1,2,3,6}[(n-1)%5]
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _pat[5] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)6 };

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for offset 0 – not used in tests */

    idx_t block  = (n - 1) / 5;
    idx_t offset = (n - 1) % 5;

    return (seq_t)block * (seq_t)8 + _pat[offset];
}
