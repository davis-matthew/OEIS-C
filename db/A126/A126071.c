/*
 * A126071  Number of bases (2 <= b <= n+1) in which n is a palindrome.
 * https://oeis.org/A126071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{b ∈ [2, n+1] : representation of n in base b is a
 *            palindrome}|
 * Range:     u8 n <= 213, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;
    seq_t count = 0;
    for (idx_t b = 2u; b <= n + 1u; ++b)
    {
        if (is_palindrome((seq_t)n, b))
            ++count;
    }
    return count;
}
