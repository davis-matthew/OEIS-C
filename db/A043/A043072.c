/*
 * A043072  a(n)=(s(n)+8)/9, where s(n)=n-th base 9 palindrome that starts
 *          with 1.
 * https://oeis.org/A043072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (p + 8) / 9, where p is the n-th base‑9 palindrome that
 *            starts with digit 1
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   pow9_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow9_int.h"

seq_t a(idx_t n)
{
    /* Find the length L of the desired palindrome. */
    int L = 1;
    idx_t remaining = n;
    while (1) {
        int free_cnt = (L % 2 == 1) ? ((L - 1) / 2) : (L / 2 - 1);
        idx_t cnt = pow9_int(free_cnt);
        if (remaining <= cnt)
            break;
        remaining -= cnt;
        ++L;
    }

    /* remaining now is the 1‑based index within palindromes of length L. */
    idx_t idx_in_len = remaining;

    /* Extract the free digits in base‑9 (with leading zeros). */
    int free_cnt = (L % 2 == 1) ? ((L - 1) / 2) : (L / 2 - 1);
    idx_t m = idx_in_len - 1;
    uint8_t free_digits[10] = {0};
    for (int i = free_cnt - 1; i >= 0; --i) {
        free_digits[i] = (uint8_t)(m % 9);
        m /= 9;
    }

    /* Build the full palindrome. */
    uint8_t digits[20];
    digits[0] = 1;
    digits[L - 1] = 1;
    for (int i = 0; i < free_cnt; ++i) {
        digits[i + 1] = free_digits[i];
        digits[L - 2 - i] = free_digits[i];
    }

    /* Compute the palindrome value. */
    seq_t p = 0;
    for (int i = 0; i < L; ++i)
        p = p * (seq_t)9 + (seq_t)digits[i];

    /* Compute a(n). */
    return (p + 8) / 9;
}
