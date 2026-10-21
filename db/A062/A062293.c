/*
 * A062293  Smallest multiple k*n of n which has even digits and is a
 *          palindrome or becomes a palindrome when 0's are added on the left
 *          (e.g., 10 becomes 010, which is a palindrome).
 * https://oeis.org/A062293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the least positive multiple k*n of n such that (1) every
 *            decimal digit of k*n is even (0,2,4,6,8), (2) k*n is a
 *            palindrome, or it can be turned into a palindrome by adding
 *            leading zeroes (e.g. 20 → 020). a(0)=0
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   padded_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple search over multiples of n and
   tests the digit conditions.  All helper routines are written
   directly and use only integer arithmetic.  */
#include <stdbool.h>
#include "../utils/padded_palindrome.h"

static bool all_even_digits(seq_t x)
{
    if (x == 0) return true;
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d & 1) return false;            /* odd digit found */
        x /= 10;
    }
    return true;
}

static bool is_palindrome(seq_t x)
{
    /* store digits in an array */
    uint8_t digits[64];
    int len = 0;
    seq_t t = x;
    do {
        digits[len++] = (uint8_t)(t % 10);
        t /= 10;
    } while (t != 0);

    for (int i = 0; i < len / 2; ++i)
        if (digits[i] != digits[len - 1 - i]) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* definition at offset 0 */

    /* search k from 1 upward */
    for (idx_t k = 1; ; ++k) {
        seq_t m = (seq_t)n * (seq_t)k;
        if (!all_even_digits(m)) continue;
        if (is_palindrome(m) || padded_palindrome(m))
            return m;
    }
}
