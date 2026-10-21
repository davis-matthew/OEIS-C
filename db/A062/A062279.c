/*
 * A062279  Smallest multiple k*n of n which is a palindrome or becomes a
 *          palindrome when 0's are added on the left (e.g. 10 becomes 010
 *          which is a palindrome).
 * https://oeis.org/A062279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = the smallest positive integer k ≥ 1 such that k*n, when
 *            leading zeros are optionally added, is a palindrome
 * Range:     u8 n <= 12, u16 n <= 80, u32 n <= 161, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome_seq_t.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0. */
    if (n == 0)
        return (seq_t)0;

    const seq_t n_seq = (seq_t)n;
    idx_t k = 1;

    for (;; ++k)
    {
        seq_t m = n_seq * (seq_t)k;          /* Current multiple k*n. */
        seq_t prefix = m;                    /* Strip trailing zeros. */

        while (prefix % (seq_t)10 == 0)
            prefix /= (seq_t)10;

        if (is_palindrome_seq_t(prefix))
            return m;                       /* Found the smallest such multiple. */
    }
}
