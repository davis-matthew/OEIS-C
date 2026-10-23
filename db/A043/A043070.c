/*
 * A043070  a(n)=(s(n)+2)/8, where s(n)=n-th base 8 palindrome that starts
 *          with 6 (in base 8), written in decimal digits.
 * https://oeis.org/A043070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+2)/8, where s(n) is the n-th base‑8 palindrome that
 *            starts with the digit 6 (in base‑8). The palindrome is
 *            interpreted as a base‑8 number and converted to decimal; the
 *            result s(n) is then transformed to a(n). The implementation
 *            generates the required palindrome directly from the index n
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires: seq_t and idx_t defined by the build system. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle the first two palindromes explicitly. */
    if (n == 1) return 1; /* "6"  octal → 6 decimal → (6+2)/8 = 1 */
    if (n == 2) return 7; /* "66" octal → 54 decimal → (54+2)/8 = 7 */

    idx_t n1 = n - 2;          /* position within the groups of longer palindromes */
    idx_t k = 2;               /* half‑length counter (for lengths 2k–1 and 2k) */

    /* Find the length group that contains the n‑th palindrome. */
    while (true) {
        /* Count of palindromes per length for this k: 8^(k‑1). */
        idx_t pow = 1;
        for (idx_t i = 0; i < k - 1; i++) pow *= 8;

        if (n1 <= pow) {                /* odd length 2k‑1 */
            idx_t offset = n1 - 1;      /* 0‑based index within this group */

            /* Convert offset to base‑8 digits (k‑1 positions). */
            uint8_t digits[64];
            for (idx_t i = k - 2; i + 1 > 0; i--) {
                digits[i] = offset % 8;
                offset /= 8;
            }

            /* Prefix digits: first is always 6. */
            uint8_t prefix[64];
            prefix[0] = 6;
            for (idx_t i = 0; i < k - 1; i++) {
                prefix[i + 1] = digits[i];
            }

            /* Build the full palindrome in base‑8 and convert to decimal. */
            seq_t val = 0;
            for (idx_t i = 0; i < k; i++) {
                val = val * 8 + prefix[i];
            }
            /* Mirror excluding the middle digit. */
            for (idx_t i = k - 2; (int)i >= 0; i--) {
                val = val * 8 + prefix[i];
            }

            return (val + 2) / 8;
        }

        n1 -= pow;                 /* skip this odd length block */

        if (n1 <= pow) {            /* even length 2k */
            idx_t offset = n1 - 1;   /* 0‑based index within this group */

            uint8_t digits[64];
            for (idx_t i = k - 2; i + 1 > 0; i--) {
                digits[i] = offset % 8;
                offset /= 8;
            }

            uint8_t prefix[64];
            prefix[0] = 6;
            for (idx_t i = 0; i < k - 1; i++) {
                prefix[i + 1] = digits[i];
            }

            seq_t val = 0;
            for (idx_t i = 0; i < k; i++) {
                val = val * 8 + prefix[i];
            }
            /* Mirror all digits (including the leading 6). */
            for (idx_t i = k - 1; (int)i >= 0; i--) {
                val = val * 8 + prefix[i];
            }

            return (val + 2) / 8;
        }

        n1 -= pow;                 /* skip this even length block */
        k++;                       /* move to the next pair of lengths */
    }
}
