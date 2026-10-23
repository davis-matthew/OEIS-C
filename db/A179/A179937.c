/*
 * A179937  a(n) is the product of the non-palindromic divisors of n.
 * https://oeis.org/A179937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the non‑palindromic divisors of n
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 119, u64 n <= 239,
 *            u128 n <= 839, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    /* iterate through all divisors up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d != 0)
            continue;

        /* check divisor d */
        seq_t sd = (seq_t)d;
        if (!is_palindrome(sd, 10))
            prod *= sd;

        /* check paired divisor n/d if different */
        idx_t e = n / d;
        if (e != d)
        {
            seq_t se = (seq_t)e;
            if (!is_palindrome(se, 10))
                prod *= se;
        }
    }
    return prod;
}
