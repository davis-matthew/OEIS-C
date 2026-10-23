/*
 * A132469  a(n) = (2^(5*n) - 1)/31.
 * https://oeis.org/A132469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32^n - 1)/31 = sum_{k=0}^{n-1} 32^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum   = (seq_t)0;
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i <= n; ++i) {
        if (i) {
            sum += power;      /* add 32^(i-1)   */
            power *= (seq_t)32;/* prepare 32^i   */
        }
    }
    return sum;
}
