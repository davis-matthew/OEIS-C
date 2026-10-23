/*
 * A043063  a(n)=(s(n)+2)/7, where s(n)=n-th base 7 palindrome that starts
 *          with 5.
 * https://oeis.org/A043063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+2)/7 where s(n) is the nth base 7 palindrome
 *            starting with 5
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow7_u8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow7_u8.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* behaviour for indices below offset is irrelevant */
        return 0;

    /* Find the length of the n‑th palindrome. */
    idx_t len = 1;
    seq_t rem = n;        /* remaining index to locate in current length block */

    while (1) {
        int exp = (int)((len - 1) / 2);        /* number of free digits for this length */
        seq_t cnt = pow7_u8(exp);              /* count of palindromes of this length */
        if (rem <= cnt)                        /* last block found */
            break;
        rem -= cnt;                            /* skip this whole block */
        ++len;
    }

    /* Index within the block of this length (0‑based). */
    seq_t offset = rem - 1;

    /* Number of free digits (excluding the leading and trailing 5). */
    int var = (int)((len - 1) / 2);

    /* Convert the offset to base‑7 digits (most significant first). */
    uint8_t digits[64] = {0};
    seq_t tmp = offset;
    for (int i = var - 1; i >= 0; --i) {
        digits[i] = (uint8_t)(tmp % 7);
        tmp /= 7;
    }

    /* Build the full palindrome in base‑7. */
    uint8_t full[128] = {0};
    full[0] = 5;
    full[len - 1] = 5;
    for (int i = 0; i < var; ++i)
        full[1 + i] = digits[i];
    for (int i = 0; i < var; ++i)
        full[len - 2 - i] = digits[i];         /* mirror of left half */

    /* Convert the base‑7 number to decimal. */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i)
        val = val * 7 + full[i];

    return (val + 2) / 7;
}
