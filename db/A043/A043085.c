/*
 * A043085  a(n)=(s(n)+4)/10, where s(n)=n-th base 10 palindrome that starts
 *          with 6.
 * https://oeis.org/A043085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns a(n) = (s(n)+4)/10 , where s(n) is the n-th base‐10
   palindrome that begins with the digit 6. */

#include <stdbool.h>

static idx_t pow10_idx(idx_t e)
{
    idx_t r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the length of the palindrome. */
    idx_t len = 0;
    idx_t cum = 0;
    for (idx_t L = 1; ; ++L)
    {
        idx_t half = (L + 1) / 2;
        idx_t count = pow10_idx(half - 1);   /* 10^(half-1) palindromes of length L */
        if (cum + count >= n) { len = L; break; }
        cum += count;
    }

    /* Position of the desired palindrome within its length group. */
    idx_t offset = n - cum - 1;          /* zero‑based */

    /* Build the left half of the palindrome. */
    idx_t half = (len + 1) / 2;
    unsigned char left[20];              /* enough for lengths up to 39 */
    left[0] = 6;
    for (idx_t i = 1; i < half; ++i)
    {
        idx_t div = pow10_idx(half - 1 - i);
        left[i] = (unsigned char)((offset / div) % 10);
    }

    /* Construct the full palindrome as a sequence of digits. */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i)
    {
        unsigned char digit;
        if (i < half) digit = left[i];
        else digit = left[len - 1 - i];
        val = val * 10 + (seq_t)digit;
    }

    /* Compute (pal+4)/10. */
    return (val + 4) / 10;
}
