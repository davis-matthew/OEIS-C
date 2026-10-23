/*
 * A043026  Base-8 palindromes that start with 6.
 * https://oeis.org/A043026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043026: Base-8 palindromes that start with 6.  
 * The terms are the decimal values of all base-8 palindromes whose
 * most significant digit is 6, listed in increasing order.  The
 * function constructs the appropriate base-8 digits and converts
 * them to decimal without any lookup table. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but guard for n==0 */

    idx_t remaining = n;           /* how many yet to skip */
    idx_t L = 1;                   /* current length in base-8 digits */

    /* find the length L that contains the n-th palindrome */
    for (;;) {
        idx_t h = (L + 1) / 2;     /* half-length (ceil)  */
        idx_t count = 1;           /* number of palindromes of this length */
        for (idx_t i = 1; i < h; ++i)
            count *= (idx_t)8;     /* 8^(h-1) free middle digits */

        if (remaining <= count)
            break;

        remaining -= count;
        ++L;
    }

    /* remaining now holds the index (1-based) inside length L */
    idx_t h = (L + 1) / 2;
    idx_t remainingCount = h - 1;  /* number of free digits after the leading 6 */
    idx_t offset = remaining - 1;  /* 0‑based offset among those free digits */

    /* store the half of the digits (including the leading 6) */
    uint8_t half[64];              /* L will never exceed 64 for n up to 10⁴ */
    uint8_t digits[64];

    half[0] = 6;                   /* leading digit fixed to 6 */

    /* fill the remaining half digits from most to least significant */
    for (idx_t i = remainingCount; i > 0; --i) {
        half[i] = (uint8_t)(offset % 8);
        offset /= 8;
    }

    /* reflect the half to form the full palindrome */
    for (idx_t i = 0; i < L; ++i) {
        if (i < h)
            digits[i] = half[i];
        else
            digits[i] = half[L - 1 - i];
    }

    /* convert the base-8 digits to a decimal value */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
        val = val * (seq_t)8 + (seq_t)digits[i];

    return val;
}
