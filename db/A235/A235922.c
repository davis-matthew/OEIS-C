/*
 * A235922  Minimal k > 1 such that the base-k representation of the n-th
 *          palindrome in A002113, read in decimal, is also palindrome.
 * https://oeis.org/A235922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal k > 1 such that the base‑k representation of the
 *            * n‑th decimal palindrome, read in decimal, is a palindrome
 * Range:     u8 n <= 35, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The first palindrome counted is 0 (offset 1).  Since a base ≤ 10 always
 * works (base 10 gives the original palindrome), we scan bases 2–10. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Find the n‑th decimal palindrome (0 is the first). */
    seq_t candidate = 0;
    idx_t count = 0;

    while (count < n) {
        if (is_palindrome(candidate, 10)) {
            ++count;
            if (count == n)
                break;
        }
        ++candidate;
    }

    /* Determine the minimal base k > 1 such that the base‑k digits are a
     * palindrome when read as decimal digits.  k ≤ 10 always works. */
    for (int k = 2; k <= 10; ++k) {
        seq_t tmp = candidate;
        uint8_t digits[64];
        idx_t len = 0;

        if (tmp == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (tmp != 0) {
                digits[len++] = (uint8_t)(tmp % (seq_t)k);
                tmp /= (seq_t)k;
            }
        }

        /* Check if the digits form a palindrome. */
        idx_t i = 0, j = len - 1;
        bool pal = true;
        while (i < j) {
            if (digits[i] != digits[j]) {
                pal = false;
                break;
            }
            ++i;
            --j;
        }
        if (pal)
            return (seq_t)k;
    }

    return (seq_t)10;  /* Base 10 is guaranteed to work. */
}
