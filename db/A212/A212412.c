/*
 * A212412  Parity of curling number of binary expansion of n.
 * https://oeis.org/A212412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = parity of the curling number of the binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Determine the length of the binary representation */
    idx_t len;
    if (n == 0)
        len = 1;
    else {
        len = 0;
        idx_t tmp = n;
        while (tmp) {
            ++len;
            tmp >>= 1;
        }
    }

    /* Store bits (MSB first) */
    unsigned char bits[64];  /* 64 is enough for any 64‑bit integer */
    for (idx_t i = 0; i < len; ++i)
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1U);

    /* Compute the curling number */
    idx_t max_k = 0;
    for (idx_t m = 1; m <= len; ++m) {
        idx_t k = 1;
        idx_t start = len - m;   /* start index of last block */
        /* Count how many equal blocks precede the last one */
        while (start >= m) {
            bool equal = true;
            for (idx_t p = 0; p < m; ++p) {
                if (bits[start - m + p] != bits[start + p]) {
                    equal = false;
                    break;
                }
            }
            if (!equal)
                break;
            ++k;
            start -= m;
        }
        if (k > max_k)
            max_k = k;
    }

    /* Return parity */
    return (seq_t)(max_k & 1U);
}
