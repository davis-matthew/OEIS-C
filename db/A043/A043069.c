/*
 * A043069  a(n)=(s(n)+3)/8, where s(n)=n-th base 8 palindrome that starts
 *          with 5.
 * https://oeis.org/A043069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+3)/8, where s(n) is the n-th base‑8 palindrome
 *            starting with 5
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the length of the palindrome that contains the n‑th element. */
    idx_t len = 1;
    idx_t rank = n;   /* rank within the current length group (1‑based) */

    /* Accumulate counts of palindromes by length. */
    while (1) {
        idx_t m = (len + 1) / 2;          /* number of independent digits in the first half */
        idx_t free = m - 1;               /* free digits after the leading '5' */
        idx_t count = 1;
        for (idx_t i = 0; i < free; ++i)
            count *= (idx_t)8;            /* 8^free */

        if (rank > count) {
            rank -= count;
            ++len;
        } else {
            break;
        }
    }

    /* Build the palindrome for this length and rank. */
    idx_t m = (len + 1) / 2;
    idx_t free = m - 1;
    idx_t offset = rank - 1;              /* zero‑based index among this length */

    /* Use a small fixed array; length never exceeds 64 for 64‑bit n. */
    uint8_t digits[128] = {0};
    digits[0] = 5;                         /* leading digit */

    /* Assign free digits (indices 1 .. m‑1) from least significant to most. */
    int pos = (int)m - 1;
    while (pos >= 1) {
        digits[pos] = (uint8_t)(offset % 8);
        offset /= 8;
        --pos;
    }

    /* Mirror the first half onto the second half. */
    for (idx_t i = m; i < len; ++i)
        digits[i] = digits[len - 1 - i];

    /* Convert the base‑8 number to decimal in seq_t. */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i)
        val = val * ((seq_t)8) + (seq_t)digits[i];

    /* a(n) = (s(n)+3)/8 */
    return (val + ((seq_t)3)) / ((seq_t)8);
}
