/*
 * A043058  a(n)=(s(n)+1)/6, where s(n)=n-th base 6 palindrome that starts
 *          with 5.
 * https://oeis.org/A043058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+1)/6, where s(n) is the n‑th base‑6 palindrome that
 *            starts with 5
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle offset; n >= 1 */
    idx_t remaining = n;
    idx_t len = 1;

    /* Find the length of the n-th palindrome. */
    while (1) {
        idx_t free = (len - 1) / 2;          /* number of free digits */
        idx_t count = 1;
        for (idx_t i = 0; i < free; ++i) {
            count *= 6;
        }
        if (remaining <= count)
            break;
        remaining -= count;
        ++len;
    }

    /* Length 1: the only palindrome is "5" (decimal 5). */
    if (len == 1)
        return (seq_t)1;                      /* (5+1)/6 = 1 */

    idx_t free = (len - 1) / 2;
    idx_t index = remaining - 1;              /* 0‑based index inside this length */

    /* Decode the free digits in base‑6 (most significant first). */
    unsigned char digits[64];                 /* enough for any length that fits in idx_t */
    for (idx_t i = 0; i < free; ++i) {
        digits[free - 1 - i] = (unsigned char)(index % 6);
        index /= 6;
    }

    /* Assemble the decimal value of the palindrome. */
    seq_t val = 5;                            /* leading digit */
    idx_t left_len = free - (len % 2);        /* number of free digits on the left of the middle */

    for (idx_t i = 0; i < left_len; ++i) {
        val = val * 6 + digits[i];
    }

    if (len % 2 == 1) {                      /* middle digit for odd length */
        val = val * 6 + digits[free - 1];
    }

    for (idx_t i = left_len; i-- > 0; ) {
        val = val * 6 + digits[i];
    }

    val = val * 6 + 5;                       /* mirror the leading 5 */

    /* Resulting term a(n) = (s(n)+1)/6 */
    return (val + 1) / 6;
}
