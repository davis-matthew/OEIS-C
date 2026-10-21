/*
 * A065531  Number of palindromes in all base b representations for n, for
 *          2<=b<=n.
 * https://oeis.org/A065531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 248, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065531: number of palindromes in base‑b representations
   of n for all bases 2 ≤ b ≤ n.
   Convention: a(1) = 1. */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;           /* convention for n = 1 */

    seq_t count = 0;
    for (idx_t b = 2; b <= n; ++b)
        if (is_palindrome((seq_t)n, b))
            ++count;

    return count;
}
