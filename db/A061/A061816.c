/*
 * A061816  Obtain m by omitting trailing zeros from n (cf. A004151); a(n) =
 *          smallest multiple k*m which is a palindrome.
 * https://oeis.org/A061816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest palindrome multiple of m(n), where m(n) = n with
 *            trailing zeros removed (A004151). a(0) = 0. For n > 0, a(n) =
 *            min { k·m(n) : k ≥ 1 and k·m(n) is a decimal palindrome }. The
 *            helper function `is_palindrome_seq_t` (from utils) is used for
 *            the palindrome test
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_palindrome_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome_seq_t.h"

seq_t a(idx_t n)
{
    /* Handle the offset value a(0) = 0 */
    if (n == 0)
        return 0;

    /* Strip trailing zeros to obtain m(n) */
    seq_t m = (seq_t)n;
    const seq_t TEN = (seq_t)10;
    while ((m % TEN) == 0)
        m /= TEN;

    /* Search for the smallest k such that k·m is a palindrome */
    idx_t k = 1;
    for (;; ++k) {
        seq_t prod = m * (seq_t)k;
        if (is_palindrome_seq_t(prod))
            return prod;
    }
}
