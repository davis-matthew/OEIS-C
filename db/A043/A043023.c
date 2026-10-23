/*
 * A043023  Base 8 palindromes that start with 3.
 * https://oeis.org/A043023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base‑8 palindromes that start with 3, in increasing order. */

#include <stdint.h>
#include <stddef.h>

#ifndef MAX_LEN
#define MAX_LEN 32   /* maximum length needed for n up to 10000 */
#endif

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;      /* offset not used */
    }

    /* Find the length L of the n‑th palindrome. */
    idx_t cumulative = 0;
    idx_t L = 1;
    while (1) {
        idx_t free_count = (L - 1) / 2;          /* number of free digits */
        idx_t count = 1;                         /* number of palindromes of this length */
        for (idx_t i = 0; i < free_count; ++i) {
            count *= 8;                          /* 8^free_count  */
        }
        if (cumulative + count >= n) {
            break;
        }
        cumulative += count;
        ++L;
    }

    /* Offset within the block of palindromes of length L. */
    idx_t offset = n - cumulative - 1;           /* 0‑based */

    /* Build the base‑8 digits of the palindrome. */
    uint8_t digits[MAX_LEN] = {0};
    digits[0] = 3;                              /* most significant digit */
    digits[L - 1] = 3;                          /* least significant digit is also 3 */

    /* Fill the free digits (most significant to least). */
    idx_t free_count = (L - 1) / 2;
    idx_t remain = offset;                      /* copy to avoid destroying offset */
    for (idx_t pos = free_count; pos > 0; --pos) {
        digits[pos] = remain % 8;
        remain /= 8;
    }

    /* Mirror the free digits to the second half. */
    for (idx_t i = 1; i <= free_count; ++i) {
        digits[L - 1 - i] = digits[i];
    }

    /* Convert the base‑8 digit array to a seq_t value. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        val = val * 8 + digits[i];
    }

    return val;
}
