/*
 * A043020  Base-7 palindromes that start with 6.
 * https://oeis.org/A043020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base‑7 palindromes beginning with digit 6, enumerated in ascending order.
   a(1) = 6, a(2) = 48, a(3) = 300, … */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */

    idx_t len = 1;          /* current palindrome length */ 
    idx_t cum = 0;          /* number of palindromes with length < len */

    /* find minimal length with cum + count >= n */
    while (1) {
        idx_t half = (len + 1) / 2;          /* ceil(len/2) */
        idx_t remaining = n - cum;           /* how many more we need */
        idx_t count = 1;                     /* 7^(half-1) */

        /* compute 7^(half-1) but stop if it would exceed 'remaining' */
        for (idx_t i = 0; i < half - 1; i++) {
            if (count > remaining / 7) {
                count = remaining + 1; /* set to larger than needed */
                break;
            }
            count *= 7;
        }

        if (cum + count >= n) break;  /* desired length found */
        cum += count;
        len++;
    }

    /* index within the current length (0‑based) */
    idx_t idx = n - cum - 1;

    /* digits of the first half: first digit is always 6 */
    uint8_t half_digits[64] = {0};
    idx_t half = (len + 1) / 2;
    half_digits[0] = 6;

    /* fill remaining half-1 digits from idx (base‑7, padded with zeros) */
    idx_t rem = idx;
    for (idx_t pos = half - 1; pos > 0; pos--) {
        half_digits[pos] = rem % 7;
        rem /= 7;
    }

    /* construct full digit array */
    uint8_t digits[128];
    for (idx_t i = 0; i < half; i++) digits[i] = half_digits[i];
    if (len % 2 == 0) {          /* even length: mirror whole half */
        for (idx_t i = 0; i < half; i++)
            digits[half + i] = half_digits[half - 1 - i];
    } else {                     /* odd length: mirror excluding middle */
        for (idx_t i = 0; i < half - 1; i++)
            digits[half + i] = half_digits[half - 2 - i];
    }

    /* convert base‑7 digits to decimal value */
    seq_t res = 0;
    for (idx_t i = 0; i < len; i++) {
        res = res * 7 + (seq_t)digits[i];
    }
    return res;
}
