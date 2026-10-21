/*
 * A082744  Smallest k such that n*k + 1 is a palindrome.
 * https://oeis.org/A082744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that n*k + 1 is a decimal palindrome
 * Range:     u8 n <= 22, u16 n <= 90, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Test whether x is a decimal palindrome.
   Works for all supported seq_t types (fixed‑size or big integer). */
static bool is_pal(seq_t x)
{
    if (x == 0)
        return true;

    /* A fixed buffer that is large enough for all expected test values
       (10000 terms and the modest sizes of the corresponding palindromes). */
    static const size_t MAX_DIGITS = 256;
    unsigned char digits[MAX_DIGITS];
    size_t len = 0;
    seq_t temp = x;
    const seq_t ten = (seq_t)10;

    /* Extract decimal digits. */
    while (temp > 0) {
        /* temp % 10 and temp /= 10 are supported for all seq_t types. */
        digits[len++] = (unsigned char)(temp % ten);
        temp /= ten;
        if (len == MAX_DIGITS)
            break;  /* safety fallback */
    }

    /* Check palindrome. */
    for (size_t i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i])
            return false;
    }
    return true;
}

/* core sequence function */
seq_t a(idx_t n)
{
    const seq_t sn = (seq_t)n;
    for (idx_t k = 1;; ++k) {
        seq_t m = sn * (seq_t)k + 1;
        if (is_pal(m))
            return (seq_t)k;
    }
}
