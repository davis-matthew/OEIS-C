/*
 * A212529  Negative numbers in base -2.
 * https://oeis.org/A212529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal integer formed from the negative base –2
 *            representation of –n (digits 0/1)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 682, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence starts at offset 1, so a(0) is undefined. */
    if (n == 0) return 0;

    /* Use a signed 128‑bit type for the division algorithm. */
    __int128 val = -((__int128)n);

    /* Store the digits 0/1 (least significant first). */
    unsigned char digits[256];
    size_t len = 0;

    while (val != 0) {
        __int128 r = val % (-(__int128)2);  /* remainder after division by –2 */
        val /= -(__int128)2;
        if (r < 0) {                /* adjust to a non‑negative remainder */
            r += 2;
            val += 1;
        }
        digits[len++] = (unsigned char)r;
    }

    /* Build the decimal number from the most significant digit. */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        res = res * 10 + (seq_t)digits[len - 1 - i];
    }

    return res;
}
