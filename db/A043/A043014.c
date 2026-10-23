/*
 * A043014  Base-6 palindromes that start with 5.
 * https://oeis.org/A043014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A043014: base‑6 palindromes that start with 5.
   a(n) is the n‑th such palindrome in increasing order. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the length L of the target palindrome. */
    idx_t rem = n;          /* remaining 1‑based index */
    idx_t L   = 1;          /* current length (base‑6 digits) */

    while (1) {
        unsigned long long count64;

        if (L == 1) {
            count64 = 1;
        } else if (L % 2 == 0) {         /* even length  L = 2m */
            idx_t m = L / 2;
            count64 = 1;
            for (idx_t i = 0; i < m - 1; ++i) count64 *= 6ull;
        } else {                          /* odd length  L = 2m+1 */
            idx_t m = (L - 1) / 2;
            count64 = 1;
            for (idx_t i = 0; i < m; ++i) count64 *= 6ull;
        }

        if (rem > count64) {
            rem -= count64;
            ++L;
        } else {
            break;
        }
    }

    /* offset within the group of length L (0‑based) */
    seq_t offset = (seq_t)(rem - 1);

    /* Number of free digits (those not fixed to 5 or mirrored). */
    idx_t fp = (L % 2 == 0) ? (L / 2 - 1) : ((L - 1) / 2);

    /* Extract the free digits from offset in base‑6, most significant first. */
    unsigned char d[32] = {0};
    for (idx_t i = fp; i > 0; --i) {
        d[i - 1] = (unsigned char)(offset % 6);
        offset /= 6;
    }

    /* Build the value by processing digits from left to right. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        unsigned char digit;

        if (i == 0 || i == L - 1) {
            digit = 5;                          /* fixed ends */
        } else if ((L % 2 == 1) && (i == (L - 1) / 2)) {
            digit = d[fp - 1];                  /* middle digit for odd length */
        } else {
            idx_t idx;
            if (i <= (L - 1) / 2) {
                idx = i - 1;                     /* left side */
            } else {
                idx = (L - 1 - i) - 1;           /* mirrored right side */
            }
            digit = d[idx];
        }

        val = val * 6 + (seq_t)digit;
    }

    return val;
}
