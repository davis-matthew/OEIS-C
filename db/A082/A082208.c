/*
 * A082208  Palindromes for which the sum of the digits is also a palindrome.
 * https://oeis.org/A082208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th decimal palindrome whose digit sum is also a
 *            palindrome (with offset 1, i.e. a(1)=0). The algorithm
 *            enumerates all decimal palindromes in increasing order and tests
 *            the digit-sum condition with the helper functions digit_sum()
 *            and is_pal()
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum, is_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_pal.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                     // number of qualifying palindromes seen

    for (int len = 1; ; ++len) {         // iterate over palindrome length
        int half_len = (len + 1) / 2;     // number of digits of the left half

        seq_t start, end;

        if (len == 1) {                  // single‑digit palindromes: 0‑9
            start = 0;
            end   = 9;
        } else {                         // length ≥ 2 : left half cannot start with 0
            start = 1;
            for (int i = 1; i < half_len; ++i) {
                start *= 10;             // start = 10^(half_len-1)
            }
            end = start * 10 - 1;        // last possible half
        }

        for (seq_t half = start; ; ++half) {
            /* Build full palindrome from the left half */
            seq_t palindrome = half;
            seq_t rev = half;

            if (len & 1) {               // odd length -- drop middle digit for mirror
                rev /= 10;
            }

            while (rev > 0) {
                seq_t digit = rev % 10;
                palindrome = palindrome * 10 + digit;
                rev /= 10;
            }

            /* Check whether the digit sum is a palindrome */
            if (is_pal(digit_sum(palindrome))) {
                ++count;
                if (count == n) {
                    return palindrome;
                }
            }

            if (half == end) {
                break;                    // finished this length
            }
        }
    }
}
