/*
 * A043060  a(n)=(s(n)+5)/7, where s(n)=n-th base 7 palindrome that starts
 *          with 2.
 * https://oeis.org/A043060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+5)/7, where s(n) is the n-th base-7 palindrome that
 *            starts with 2. This code computes s(n) by enumerating
 *            palindromes by length, then returns (s(n)+5)/7
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    idx_t cum = 0;            /* number of palindromes already counted */
    idx_t block = 0;          /* current block index k (free digits count) */
    idx_t blockCount = 1;     /* 7^k */

    /* Find the block and whether the palindrome is odd or even length */
    while (1) {
        /* Check odd length part (L = 2k+1) */
        if (n <= cum + blockCount) {
            idx_t offset = n - cum - 1;          /* 0‑based offset within this block */
            /* Build palindrome of length L = 2*k+1 */
            const int len = (int)(2 * block + 1);
            uint8_t pal[12];                     /* maximum length is 12 for n ≤ 10000 */
            unsigned int dig[12];                /* digits of offset in base 7 (pad) */

            /* Extract base-7 digits of offset, padded with leading zeros */
            idx_t tmp = offset;
            for (int i = block - 1; i >= 0; --i) {
                dig[i] = tmp % 7;
                tmp /= 7;
            }

            /* Place digits into palindrome */
            pal[0] = 2;
            for (int i = 1; i <= block; ++i)
                pal[i] = dig[i - 1];
            for (int i = block + 1; i < len; ++i)
                pal[i] = pal[2 * block - i];

            /* Convert to decimal */
            seq_t val = 0;
            for (int i = 0; i < len; ++i)
                val = val * 7 + pal[i];

            return (val + 5) / 7;
        }

        /* Check even length part (L = 2k+2) */
        if (n <= cum + 2 * blockCount) {
            idx_t offset = n - cum - blockCount - 1;   /* 0‑based offset within even part */
            /* Build palindrome of length L = 2*k+2 */
            const int len = (int)(2 * block + 2);
            uint8_t pal[12];
            unsigned int dig[12];

            idx_t tmp = offset;
            for (int i = block - 1; i >= 0; --i) {
                dig[i] = tmp % 7;
                tmp /= 7;
            }

            pal[0] = 2;
            for (int i = 1; i <= block; ++i)
                pal[i] = dig[i - 1];
            for (int i = block + 1; i < len; ++i)
                pal[i] = pal[len - 1 - i];

            seq_t val = 0;
            for (int i = 0; i < len; ++i)
                val = val * 7 + pal[i];

            return (val + 5) / 7;
        }

        /* Advance to next block */
        cum += 2 * blockCount;
        ++block;
        blockCount *= 7;
    }
}
