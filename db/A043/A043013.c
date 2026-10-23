/*
 * A043013  Base-6 palindromes that start with 4.
 * https://oeis.org/A043013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow6k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate base‑6 palindromes that start with the digit 4 */
#include <stdint.h>
#include "../utils/pow6k.h"

/* a(n) = n-th base‑6 palindrome beginning with 4 (OEIS A043013) */
seq_t a(idx_t n) {
    idx_t remaining = n;
    idx_t length = 1;

    /* Find the length of the palindrome that contains the n‑th element */
    while (1) {
        idx_t k, count_len;
        if ((length & 1) == 1) {            /* odd length */
            k = (length - 1) >> 1;
            count_len = (idx_t)pow6k(k);
        } else {                            /* even length */
            k = length >> 1;
            count_len = (idx_t)pow6k(k - 1);
        }
        if (remaining <= count_len) break;

        remaining -= count_len;
        ++length;
    }

    /* 0‑based index within the found length */
    idx_t idx_within = remaining - 1;

    /* Parameters for constructing the palindrome */
    idx_t k = (length & 1) ? ((length - 1) >> 1) : (length >> 1);
    idx_t free_cnt = (length & 1) ? k : (k - 1);   /* number of free inner digits */

    /* Build the digit array (digits are 0‑based, most significant first) */
    uint8_t digits[length];
    for (idx_t i = 0; i < length; ++i) digits[i] = 0;
    digits[0] = 4;                                 /* first digit fixed to 4 */

    /* Fill free inner digits from the base‑6 representation of idx_within */
    if (free_cnt > 0) {
        idx_t tmp = idx_within;
        for (idx_t i = free_cnt; i > 0; --i) {
            digits[i] = (uint8_t)(tmp % 6);
            tmp /= 6;
        }
    }

    /* Mirror left half to the right half to complete the palindrome */
    for (idx_t i = 0; i < k; ++i) {
        digits[length - 1 - i] = digits[i];
    }

    /* Convert the base‑6 digit array to a decimal (base‑10) value */
    seq_t val = 0;
    for (idx_t i = 0; i < length; ++i) {
        val = val * 6 + digits[i];
    }

    return val;
}
