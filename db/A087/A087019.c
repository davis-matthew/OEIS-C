/*
 * A087019  Lunar squares: n*n where * is lunar multiplication (A087062).
 * https://oeis.org/A087019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * n with lunar multiplication (A087062)
 * Range:     u8 n <= 29, u16 n <= 653, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least‑significant first). */
    unsigned char digits[64];
    size_t d = 0;
    if (n == 0) {
        digits[d++] = 0;
    } else {
        for (idx_t x = n; x > 0; x /= 10) {
            digits[d++] = (unsigned char)(x % 10);
        }
    }

    /* Result can have at most 2*d-1 digits. */
    unsigned char res[128] = {0};

    for (size_t j = 0; j < d; ++j) {
        unsigned char mj = digits[j];
        for (size_t i = 0; i < d; ++i) {
            /* Partial digit (min of the two digits). */
            unsigned char p = (digits[i] < mj) ? digits[i] : mj;
            size_t pos = i + j;        /* shift by position of multiplier digit. */
            if (p > res[pos])           /* lunar addition uses max. */
                res[pos] = p;
        }
    }

    /* Trim leading zeros (most significant). */
    size_t len = 2 * d;
    while (len > 1 && res[len - 1] == 0) {
        --len;
    }

    /* Convert the digit array back to seq_t. */
    seq_t value = 0;
    for (size_t k = len; k-- > 0; ) {  /* from MSD to LSD */
        value = value * 10 + res[k];
    }

    return value;
}
