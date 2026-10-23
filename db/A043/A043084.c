/*
 * A043084  a(n)=(s(n)+5)/10, where s(n)=n-th base 10 palindrome that starts
 *          with 5.
 * https://oeis.org/A043084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+5)/10 where s(n) is the n‑th base‑10 palindrome *
 *            starting with 5 (OEIS A043084)
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pal_count, pow10_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pal_count.h"
#include "../utils/pow10_cap.h"

/* Build the n-th palindrome of a given length (1‑based offset 'off'). */
static seq_t build_pal(idx_t length, idx_t off)
{
    if (length == 1)
        return (seq_t)5;

    int len = (int)length;
    int free_digits = (len % 2 == 0) ? (len / 2 - 1) : (len / 2);
    /* digits[0] will hold the leading 5; indices 1 .. free_digits hold
     * the free digits (most significant after the leading 5). */
    int digits[128] = {0};
    digits[0] = 5;
    idx_t idx = off - 1;
    for (int i = free_digits; i >= 1; --i) {
        digits[i] = (int)(idx % 10);
        idx /= 10;
    }

    seq_t result = 0;
    if (len % 2 == 1) {              /* odd length */
        for (int i = 0; i <= free_digits; ++i)
            result = result * 10 + (seq_t)digits[i];
        for (int i = free_digits - 1; i >= 0; --i)
            result = result * 10 + (seq_t)digits[i];
    } else {                          /* even length */
        for (int i = 0; i <= free_digits; ++i)
            result = result * 10 + (seq_t)digits[i];
        for (int i = free_digits; i >= 0; --i)
            result = result * 10 + (seq_t)digits[i];
    }
    return result;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Find the length that contains the n‑th palindrome. */
    idx_t rem = n;
    idx_t length = 1;
    while (1) {
        idx_t cnt = pal_count(length, rem);
        if (rem <= cnt)
            break;
        rem -= cnt;
        ++length;
    }

    /* Build the palindrome and apply the formula. */
    seq_t pal = build_pal(length, rem);
    return (pal + (seq_t)5) / (seq_t)10;
}
