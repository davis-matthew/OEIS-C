/*
 * A043025  Base-8 palindromes that start with 5.
 * https://oeis.org/A043025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow8u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Constructs the n‑th base‑8 palindrome that starts with digit 5.
   Palindromes are generated in increasing numeric order:
   length 1,2,3,4,… and, within each length, by lexicographic
   order of the inner digits. */

#include <stddef.h>
#include "../utils/pow8u.h"

seq_t a(idx_t n)
{
    /* Find the length L of the palindrome */
    idx_t remaining = n;
    int L = 1;
    while (1)
    {
        int m = (L - 1) / 2;          /* number of free inner digits */
        idx_t cnt = 1;
        for (int i = 0; i < m; ++i) { /* cnt = 8^m   (m ≤ 5 for n ≤ 10000) */
            cnt *= 8;
        }
        if (remaining <= cnt) break;
        remaining -= cnt;
        ++L;
    }

    /* Offset within this length block (0‑based) */
    idx_t offset = remaining - 1;

    /* Extract the m inner digits in base‑8 */
    int m = (L - 1) / 2;
    int inner[m];
    int idx = m - 1;
    idx_t tmp = offset;
    while (idx >= 0)
    {
        inner[idx] = (int)(tmp % ((idx_t)8));
        tmp /= 8;
        --idx;
    }

    /* Build digit array for the full palindrome */
    int digits[20];   /* enough for all lengths we need */
    for (int i = 0; i < L; ++i) digits[i] = 5;            /* first and last are 5 */

    /* Place the inner digits */
    for (int i = 0; i < m; ++i) digits[1 + i] = inner[i];

    /* Mirror the left part */
    if (L % 2 == 1)        /* odd length – middle digit already placed */
    {
        for (int i = 0; i < m - 1; ++i)
            digits[L - 2 - i] = inner[i];
    }
    else                    /* even length */
    {
        for (int i = 0; i < m; ++i)
            digits[L - 2 - i] = inner[i];
    }

    /* Compute the numeric value in base 10 (or any desired base for seq_t) */
    seq_t val = 0;
    for (int i = 0; i < L; ++i) val = val * 8 + (seq_t)digits[i];

    return val;
}
