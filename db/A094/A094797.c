/*
 * A094797  Number of times 1 is used in writing out all numbers 1 through
 *          10^n.
 * https://oeis.org/A094797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n*10^(n-1) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t res = pow10(n - 1);   /* 10^(n-1) */
    res *= (seq_t)n;            /* n * 10^(n-1) */
    res += (seq_t)1;            /* +1 */
    return res;
}
