/*
 * A184988  Largest palindromic substring of n^2.
 * https://oeis.org/A184988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, sub_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/sub_val.h"

/* Largest palindromic substring of n².
   Compute n², then find the maximum numeric value of any substring of
   its decimal representation that is a palindrome and does not begin
   with a zero unless it is the single digit 0. */

seq_t a(idx_t n)
{
    /* n² as a seq_t value */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Extract decimal digits of sq in reverse order. */
    const size_t MAX_D = 256;          /* enough for expected range */
    uint8_t rev[MAX_D];
    size_t len;
    get_digits(sq, rev, &len);

    /* Build normal‑order digits array. */
    uint8_t digits[MAX_D];
    for (size_t i = 0; i < len; ++i)
        digits[i] = rev[len - 1 - i];

    seq_t best = 0;

    /* Odd‑length palindromes (center at each digit). */
    for (size_t center = 0; center < len; ++center) {
        size_t l = center, r = center;
        while (1) {
            if (digits[l] != digits[r]) break;
            if (!(digits[l] == 0 && r - l + 1 > 1)) {
                seq_t v = sub_val(digits, l, r);
                if (v > best) best = v;
            }
            if (l == 0 || r == len - 1) break;
            --l; ++r;
        }
    }

    /* Even‑length palindromes (center between adjacent digits). */
    for (size_t center = 0; center + 1 < len; ++center) {
        if (digits[center] != digits[center + 1]) continue;
        size_t l = center, r = center + 1;
        while (1) {
            if (digits[l] != digits[r]) break;
            if (!(digits[l] == 0 && r - l + 1 > 1)) {
                seq_t v = sub_val(digits, l, r);
                if (v > best) best = v;
            }
            if (l == 0 || r == len - 1) break;
            --l; ++r;
        }
    }

    return best;
}
