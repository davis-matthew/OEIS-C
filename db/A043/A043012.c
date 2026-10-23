/*
 * A043012  Base-6 palindromes that start with 3.
 * https://oeis.org/A043012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth base‑6 palindrome that starts with digit 3 (in
 *            increasing order)
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, return 0 as a safe value */

    idx_t cum = 0;         /* cumulative count of palindromes up to previous length */
    idx_t L = 1;           /* current length (number of base‑6 digits) */

    /* Find the length L that contains the nth term */
    while (1) {
        idx_t m   = (L + 1) / 2;          /* number of independent digits */
        idx_t free = m - 1;               /* digits that can vary freely */
        idx_t count = 1;
        for (idx_t i = 0; i < free; ++i) {
            /* if the count would exceed the remaining needed, we stop
               to avoid overflow and indicate that this length is sufficient */
            if (count > n - cum) {
                count = n - cum + 1;      /* any value > remaining */
                break;
            }
            count *= 6;
        }

        if (cum + count >= n) break; /* desired length found */
        cum += count;
        ++L;
    }

    idx_t pos = n - cum - 1;          /* zero‑based index within length L */
    idx_t m   = (L + 1) / 2;          /* independent digits for this length */
    idx_t free = m - 1;

    /* Build the independent digit sequence */
    unsigned char digits[64];         /* enough for all practical lengths */
    digits[0] = 3;                    /* leading digit is always 3 */

    idx_t tmp = pos;
    for (idx_t i = free; i >= 1; --i) {
        digits[i] = tmp % 6;
        tmp /= 6;
    }

    /* Convert the base‑6 palindrome to decimal (seq_t) */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        seq_t d = (i < m) ? digits[i] : digits[L - 1 - i];  /* mirror */
        value = value * 6 + d;
    }
    return value;
}
