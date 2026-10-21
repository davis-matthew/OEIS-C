/*
 * A083836  Palindromes of the form 6n + 1 where n is also a palindrome.
 *          palindromes arising in A083835.
 * https://oeis.org/A083836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 6*p + 1 where p is a palindrome and a(n) is also a
 *            palindrome. Implemented by enumerating palindromic p (called n
 *            in the code) in increasing order and checking the corresponding
 *            value 6*n+1 for palindromicity
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_palindrome, pow10, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/reverse_decimal.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t target) {
    if (target == 0) return 0;   /* offset is 1; n = 0 is undefined */
    idx_t count = 0;
    for (int len = 1; ; ++len) {
        int half = (len + 1) / 2;                      /* number of digits in the left half */
        idx_t len_half = (idx_t)half;
        seq_t start = pow10((idx_t)(len_half - 1));     /* first left half with this length */
        seq_t end   = pow10((idx_t)len_half) - 1;       /* last left half with this length */
        bool is_even = (len % 2 == 0);
        seq_t mult = is_even ? pow10((idx_t)len_half)
                             : pow10((idx_t)(len_half - 1));

        for (seq_t left = start; ; ++left) {
            if (left > end) break;                    /* finished this length */

            seq_t n;
            if (is_even) {
                /* Even length: palindrome = left * 10^half + reverse_decimal(left) */
                n = left * mult + reverse_decimal(left);
            } else {
                /* Odd length: palindrome = left * 10^(half-1) + reverse_decimal(left/10) */
                n = left * mult + reverse_decimal(left / 10);
            }

            seq_t m = n * 6 + 1;                      /* candidate value */
            if (is_palindrome(m, 10)) {
                ++count;
                if (count == target) return m;
            }
        }
    }
}
