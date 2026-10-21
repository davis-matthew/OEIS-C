/*
 * A083178  Numbers with a digit sum of n and a maximum product of digits. In
 *          case of two identical products choose the largest number.
 * https://oeis.org/A083178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = number with digit sum n and maximum product, which is
 *            uniquely given by k = {0,2,1} for n%3 = {0,1,2} and m =
 *            (n-2*k)/3, then a(n) = 3 repeated m times followed by 2 repeated
 *            k times. For n <= 3, a(n) = n
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)3;

    idx_t k;
    switch (n % (idx_t)3)
    {
        case (idx_t)0: k = 0; break;
        case (idx_t)1: k = 2; break;   /* n mod 3 == 1 → k=2 (two 2's) */
        default:      k = 1; break;   /* n mod 3 == 2 → k=1 (one 2) */
    }

    idx_t m = (n - 2 * k) / 3;

    seq_t result = 0;
    for (idx_t i = 0; i < m; ++i)
        result = result * 10 + 3;
    for (idx_t i = 0; i < k; ++i)
        result = result * 10 + 2;

    return result;
}
