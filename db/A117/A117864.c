/*
 * A117864  Number of palindromes (in base 5) below 5^n.
 * https://oeis.org/A117864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*5^((n-1)/2)-2 (n odd), 2*5^(n/2)-2 (n even)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n % 2 == 1)  /* n is odd */
    {
        idx_t k = (n - 1) / 2;
        return (seq_t)6 * seq_pow((seq_t)5, k) - (seq_t)2;
    }
    else            /* n is even */
    {
        idx_t k = n / 2;
        return (seq_t)2 * seq_pow((seq_t)5, k) - (seq_t)2;
    }
}
