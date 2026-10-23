/*
 * A047382  Numbers that are congruent to {0, 5} mod 7.
 * https://oeis.org/A047382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/2) + (n%2==0 ? 5 : 0) (n>=1)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res;
    if (n & 1)  /* odd */
    {
        res = 7 * ((seq_t)(n - 1) / 2);
    }
    else        /* even */
    {
        res = 7 * ((seq_t)(n - 2) / 2) + 5;
    }
    return res;
}
