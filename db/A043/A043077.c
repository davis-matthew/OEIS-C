/*
 * A043077  a(n)=(s(n)+3)/9, where s(n)=n-th base 9 palindrome that starts
 *          with 6.
 * https://oeis.org/A043077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+3)/9, where s(n) is the nth base‑9 palindrome that
 *            starts with 6
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first palindrome (single digit '6') */
    if (n == 1) return (seq_t)1;

    /* remaining counts how many palindromes after the first one */
    seq_t remaining = (seq_t)n - 1;
    int L = 2;                 /* current length of palindromes   */
    seq_t count = 1;           /* number of palindromes of length L */

    /* Find the length L of the desired palindrome */
    while (1) {
        if (remaining <= count) break;
        remaining -= count;
        ++L;
        if (L % 2 == 1)        /* every odd length increases the free digits */
            count *= (seq_t)9;
    }

    /* Build the base‑9 palindrome of length L */
    int k = (L - 1) / 2;          /* number of free digits (including middle) */
    unsigned char digits[128] = {0};

    seq_t idx_inside = remaining - 1; /* zero‑based index inside this length */

    /* Extract free digits in reverse order (base 9) */
    for (int j = k; j >= 1; --j) {
        seq_t d = idx_inside % (seq_t)9;
        digits[j] = (unsigned char)d;
        idx_inside /= (seq_t)9;
    }

    /* Mirror the free digits to obtain the full palindrome */
    for (int j = 1; j <= k; ++j) {
        int mirror = L - 1 - j;
        digits[mirror] = digits[j];
    }

    /* Leading and trailing digits are always 6 */
    digits[0] = 6;
    digits[L - 1] = 6;

    /* Convert the palindrome from base 9 to decimal */
    seq_t s_val = 0;
    for (int i = 0; i < L; ++i) {
        s_val = s_val * (seq_t)9 + (seq_t)digits[i];
    }

    /* Return the required sequence value */
    return (s_val + (seq_t)3) / (seq_t)9;
}
