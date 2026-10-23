/*
 * A043059  a(n)=(s(n)+6)/7, where s(n)=n-th base 7 palindrome that starts
 *          with 1.
 * https://oeis.org/A043059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+6)/7, where s(n) is the n-th base‑7 palindrome
 *            starting with digit 1. The function returns the n‑th term of
 *            A043059
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the length of the palindrome that contains the n-th term */
    idx_t cumulative = 0;
    idx_t len = 1;
    while (1) {
        idx_t halfLen = (len + 1) / 2;
        idx_t count = 1;                 /* number of palindromes of this length */
        for (idx_t i = 0; i < halfLen - 1; ++i) count *= 7;
        if (cumulative + count >= n) break;
        cumulative += count;
        ++len;
    }

    /* Rank of the desired palindrome within its length (1‑based) */
    idx_t rank = n - cumulative;         /* 1 ≤ rank ≤ count */
    idx_t halfLen = (len + 1) / 2;
    idx_t d = halfLen - 1;              /* number of free digits after the leading 1 */

    /* Build the left half of the palindrome */
    uint8_t leftHalf[20];                /* 20 > maximal possible half length (≤11) */
    uint8_t digits[20];
    leftHalf[0] = 1;                     /* leading digit fixed to 1 */

    idx_t r = rank - 1;                  /* zero‑based index for the free digits */
    for (idx_t i = halfLen - 1; i > 0; --i) {
        leftHalf[i] = r % 7;
        r /= 7;
    }

    /* Form the full digit sequence: left half plus mirrored right half */
    for (idx_t i = 0; i < len; ++i) {
        uint8_t val = (i < halfLen) ? leftHalf[i] : leftHalf[len - 1 - i];
        digits[i] = val;
    }

    /* Convert the base‑7 digits to a decimal value */
    seq_t value = 0;
    for (idx_t i = 0; i < len; ++i) {
        value = value * 7 + digits[i];
    }

    /* Return (value + 6) / 7 */
    return (value + 6) / 7;
}
