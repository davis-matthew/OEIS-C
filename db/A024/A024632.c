/*
 * A024632  n written in fractional base 5/2.
 * https://oeis.org/A024632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is n written in fractional base 5/2 and then read as a
 *            decimal number. The least‑significant digit of such a
 *            representation is n mod 5. The remaining part equals (n‑d)·2/5,
 *            which is always an integer. This routine extracts the digits and
 *            builds the decimal value. It works for any seq_t that...
 * Range:     u8 n <= 24, u16 n <= 249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;          /* current value while extracting digits */
    seq_t dig[64];
    unsigned int len = 0;          /* number of digits found */

    /* Special case n = 0 – no digits extracted and result remains 0. */
    while (cur > 0)
    {
        seq_t d = cur % 5;         /* current least‑significant digit */
        dig[len++] = d;

        /* Compute the next remaining value (cur - d) * 2 / 5.
           Use a wider type (uint64_t) to avoid overflow when seq_t is small
           such as uint8_t. */
        uint64_t tmp = (uint64_t)(cur - d) * 2ULL;
        cur = (seq_t)(tmp / 5ULL);
    }

    /* Build the decimal value from the digits (most significant first). */
    seq_t res = 0;
    for (unsigned int i = len; i > 0; --i)
        res = res * 10 + dig[i - 1];

    return res;
}
