/*
 * A135549  Number of bases b, 1 < b < n-1, in which n is a palindrome.
 * https://oeis.org/A135549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases b, 1 < b < n-1, in which n is a
 *            palindrome
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n <= 3) return 0;
    seq_t count = 0;
    for (idx_t b = 2; b <= n - 2; ++b)
        if (is_palindrome((seq_t)n, b))
            ++count;
    return count;
}
