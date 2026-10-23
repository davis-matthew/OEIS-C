/*
 * A044921  Numbers whose base-5 run lengths form a palindrome.
 * https://oeis.org/A044921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th smallest integer whose base‑5 run lengths form a
 *            palindrome
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   base5_runlen_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/base5_runlen_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence is 1‑based; undefined for 0 */
        return 0;

    seq_t val   = 0;
    seq_t count = 0;

    while ((idx_t)count < n) {      /* cast to idx_t for reliable comparison */
        ++val;
        if (base5_runlen_palindrome(val))
            ++count;
    }
    return val;
}
