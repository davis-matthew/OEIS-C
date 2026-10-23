/*
 * A139704  Nearly palindromic numbers: non-palindromes that can be made
 *          palindromic by inserting an extra digit.
 * https://oeis.org/A139704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   get_digits, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by brute‑force search:
   a(n) = the n‑th positive integer that is not a palindrome
   but becomes a palindrome when exactly one decimal digit
   (any of 0–9, but not 0 at the beginning) is inserted. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;
    seq_t x = (seq_t)10;                 // first term of the sequence
    static uint8_t buf[64], newbuf[65];  // buffers for digits
    size_t len, i;

    while (count < n) {
        // skip palindromes themselves
        if (!is_palindrome(x, 10)) {
            get_digits(x, buf, &len);
            bool ok = false;

            for (size_t pos = 0; pos <= len && !ok; ++pos) {          // position of insertion
                for (int d = 0; d <= 9 && !ok; ++d) {                // digit to insert
                    if (pos == 0 && d == 0) continue;                // no leading zero

                    // build candidate with inserted digit
                    for (i = 0; i < pos; ++i) newbuf[i] = buf[i];
                    newbuf[pos] = (uint8_t)d;
                    for (i = pos; i < len; ++i) newbuf[i + 1] = buf[i];
                    size_t newlen = len + 1;

                    // check palindrome property
                    bool pal = true;
                    for (i = 0; i < newlen / 2; ++i) {
                        if (newbuf[i] != newbuf[newlen - 1 - i]) {
                            pal = false;
                            break;
                        }
                    }
                    if (pal) ok = true;
                }
            }

            if (ok) {
                val = x;
                ++count;
            }
        }
        ++x;
    }
    return val;
}
