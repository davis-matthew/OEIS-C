/*
 * A091546  First column of the array A092077 ((8,2)-Stirling2).
 * https://oeis.org/A091546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>=2, a(n) = a(n-1) * 2 * (3n-2) * (6n-5)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, just return 0 */
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        /* compute the small integer multiplier safely */
        seq_t mul = (seq_t)2 *
                    (seq_t)((uint64_t)3 * i - 2) *
                    (seq_t)((uint64_t)6 * i - 5);
        res *= mul;
    }
    return res;
}
