/*
 * A088000  a(n) is the sum of the palindromic divisors of n.
 * https://oeis.org/A088000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of all divisors d of n such that d is a palindrome in
 *            base 10
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard against zero */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (is_palindrome((seq_t)d, 10))
                sum += (seq_t)d;

            idx_t other = n / d;
            if (other != d && is_palindrome((seq_t)other, 10))
                sum += (seq_t)other;
        }
    }
    return sum;
}
