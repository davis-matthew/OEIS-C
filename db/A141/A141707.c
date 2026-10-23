/*
 * A141707  Least k>0 such that (2n-1)k is palindromic in base 2.
 * https://oeis.org/A141707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = least k>0 such that (2*n-1)*k is a base-2 palindrome, k
 *            odd
 * Range:     u8 n <= 9, u16 n <= 342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for index < 1 */

    seq_t m = (seq_t)n * 2u - 1u;        /* m = 2n-1 is odd */

    for (idx_t k = 1; ; k += 2) {        /* only odd k are admissible */
        seq_t prod = m * (seq_t)k;       /* product fits comfortably in seq_t */
        if (is_palindrome(prod, 2))     /* check binary palindrome */
            return (seq_t)k;            /* minimal k found */
    }
}
