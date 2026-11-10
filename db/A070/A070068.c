/*
 * A070068  Duplicate of A068668.
 * https://oeis.org/A070068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 9; a(n) = smallest palindrome > a(n-1) that is a multiple
 *            of a(n-1) (search over multiples k*a(n-1), k = 2, 3, ...)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t cur = 9;
    idx_t i;
    for (i = 1; i < n; i++) {
        seq_t m = cur + cur;          /* 2 * cur */
        while (!is_palindrome(m))
            m += cur;
        cur = m;
    }
    return cur;
}
