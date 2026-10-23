/*
 * A043075  a(n)=(s(n)+5)/9, where s(n)=n-th base 9 palindrome that starts
 *          with 4.
 * https://oeis.org/A043075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+5)/9, where s(n) is the n‑th base‑9 palindrome that
 *            starts with the digit 4
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* zero‑based index of the desired palindrome */
    idx_t k = n - 1;

    /* Find the length of the palindrome containing the k‑th element. */
    idx_t length = 1;
    idx_t cumulative = 0;
    while (1) {
        idx_t free_positions = (length - 1) / 2;
        idx_t count = 1;
        for (idx_t i = 0; i < free_positions; ++i) count *= 9;
        if (cumulative + count > k) break;
        cumulative += count;
        ++length;
    }

    /* Position of the k‑th element within palindromes of this length. */
    idx_t free_positions = (length - 1) / 2;
    idx_t offset = k - cumulative;

    /* Build the base‑9 digits of the palindrome. */
    unsigned char digits[60] = {0};
    digits[0] = digits[length - 1] = 4;   /* first and last digit fixed to 4 */

    idx_t tmp = offset;
    for (idx_t pos = free_positions; pos > 0; --pos) {
        unsigned char d = (unsigned char)(tmp % 9);
        tmp /= 9;
        digits[pos] = d;
        digits[length - 1 - pos] = d;
    }

    /* Convert base‑9 digits to the decimal value s. */
    seq_t s = 0;
    for (idx_t i = 0; i < length; ++i) {
        s = s * 9 + (seq_t)digits[i];
    }

    /* a(n) = (s + 5) / 9. */
    return (s + 5) / 9;
}
