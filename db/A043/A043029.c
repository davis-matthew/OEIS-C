/*
 * A043029  Base-9 palindromes that start with 2.
 * https://oeis.org/A043029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 101, u32 n <= 977, u64 n <= 977,
 *            u128 n <= 977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Base-9 palindromes that start with digit 2.
   Enumerates palindromes by length (shorter first) and, for each
   length, lexicographically by the free base‑9 digits.
   The function returns a(n) as a decimal value in the type seq_t. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)2;          /* length 1 palindrome: 2 */

    /* Find the length L of the palindrome that contains the n‑th term.
       remaining holds how many palindromes are left to skip. */
    idx_t remaining = n;
    idx_t L = 1;

    while (1) {
        idx_t free = (L - (idx_t)1) / (idx_t)2;   /* number of free digits */
        idx_t cnt = (idx_t)1;                    /* 9^free */
        for (idx_t i = (idx_t)0; i < free; ++i)
            cnt *= (idx_t)9;

        if (remaining <= cnt)
            break;

        remaining -= cnt;
        ++L;
    }

    /* For L == 1 we already returned above; now L >= 2. */
    idx_t free = (L - (idx_t)1) / (idx_t)2;      /* free digits in this length */

    /* idx_zero: zero‑based index within this length (0 .. cnt-1). */
    idx_t idx_zero = remaining - (idx_t)1;

    /* Static arrays large enough for all practical n values (up to ~1e6). */
    uint8_t digits[64] = {0};
    uint8_t free_digits[64] = {0};

    /* Set the fixed digits: leading and trailing 2. */
    digits[0] = digits[L - (idx_t)1] = (uint8_t)2;

    /* Convert idx_zero to base‑9 digits for the free positions.
       The most significant free digit is placed first (at position 1). */
    idx_t tmp = idx_zero;
    for (idx_t i = (idx_t)0; i < free; ++i) {
        free_digits[free - (idx_t)1 - i] = (uint8_t)(tmp % (idx_t)9);
        tmp /= (idx_t)9;
    }

    /* Assign the free digits to the left half of the palindrome. */
    for (idx_t i = (idx_t)0; i < free; ++i)
        digits[1 + i] = free_digits[i];

    /* Mirror the left half to the right half. */
    for (idx_t i = (idx_t)0; i < free; ++i)
        digits[L - (idx_t)2 - i] = digits[1 + i];

    /* Convert the base‑9 digit array to its decimal value. */
    seq_t val = (seq_t)0;
    for (size_t i = (size_t)0; i < (size_t)L; ++i)
        val = val * (seq_t)9 + (seq_t)digits[i];

    return val;
}
