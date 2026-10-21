/*
 * A093270  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 70.
 * https://oeis.org/A093270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 7 * (10^(n-1)-1)/9 * 10^n (for n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* guard against underflow if n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* 10^(n-1) */
    seq_t p_n1 = pow10(n - 1);
    /* (10^(n-1) - 1) / 9  ==  (111...1) with (n-1) digits 1 */
    seq_t part = (p_n1 - (seq_t)1) / (seq_t)9;
    /* 10^n */
    seq_t p_n = pow10(n);
    /* 7 * part * 10^n */
    return (seq_t)7 * part * p_n;
}
