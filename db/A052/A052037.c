/*
 * A052037  Smallest positive number that must be added to n to reach a
 *          palindrome.
 * https://oeis.org/A052037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the smallest positive integer x such that n+x is a
 *            decimal palindrome
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the smallest positive integer to add to n to obtain the next decimal palindrome. */
seq_t a(idx_t n)
{
    /* Work with the integer n+1 to guarantee the resulting palindrome is greater than n. */
    uint64_t val64 = (uint64_t)n + 1ULL;            /* val = n+1, fits in uint64_t for all n used in tests */
    seq_t val = (seq_t)val64;                       /* treat as seq_t for arithmetic */

    /* Extract decimal digits of val into array 'dig' (most significant first). */
    int dig[20];                                   /* enough for all 64‑bit numbers */
    int len = 0;
    uint64_t tmp = val64;
    do {
        dig[len++] = (int)(tmp % 10ULL);
        tmp /= 10ULL;
    } while (tmp != 0ULL);
    for (int i = 0; i < len / 2; ++i) {            /* reverse to most significant first */
        int t = dig[i];
        dig[i] = dig[len - 1 - i];
        dig[len - 1 - i] = t;
    }

    /* Copy digits to produce a palindrome candidate by mirroring. */
    int pal[20];
    for (int i = 0; i < len; ++i)
        pal[i] = dig[i];
    for (int i = 0; i < len / 2; ++i)
        pal[len - 1 - i] = pal[i];

    /* Build numeric value from the palindrome digits. */
    seq_t pal_val = 0;
    for (int i = 0; i < len; ++i)
        pal_val = pal_val * 10 + (seq_t)pal[i];

    /* If the palindrome is not strictly greater than val, increment the middle. */
    if (pal_val < val) {
        int carry = 1;
        int pos;
        if (len & 1)                      /* odd length */
            pos = len / 2;
        else                              /* even length */
            pos = len / 2 - 1;

        while (pos >= 0 && carry) {
            pal[pos] += carry;
            carry = pal[pos] / 10;
            pal[pos] %= 10;
            pal[len - 1 - pos] = pal[pos];
            --pos;
        }

        /* Overflow: e.g., 999…9 -> 1000…001 */
        if (carry) {
            /* palindrome = 10^len + 1 */
            pal_val = 1;
            for (int i = 0; i < len; ++i)
                pal_val = pal_val * 10;
            pal_val += 1;
        } else {
            /* Rebuild palindrome value after increment. */
            pal_val = 0;
            for (int i = 0; i < len; ++i)
                pal_val = pal_val * 10 + (seq_t)pal[i];
        }
    }

    /* Return difference between the next palindrome and n. */
    return pal_val - (seq_t)n;
}
