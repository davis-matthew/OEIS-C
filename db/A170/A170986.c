/*
 * A170986  Carryless product n X n in base 5.
 * https://oeis.org/A170986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = carryless product of n with itself in base 5
 * Range:     u8 n <= 24, u16 n <= 255, u32 n <= 3124, u64 n <= 3124,
 *            u128 n <= 3124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Zero takes care of itself. */
    if (n == 0)
        return 0;

    /* Extract base‑5 digits of n (least significant first). */
    uint8_t dig[64];   /* plenty for any 64‑bit n */
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        dig[len++] = (uint8_t)(t % 5);
        t /= 5;
    }

    /* Convolution of the digit array with itself, modulo 5. */
    size_t res_len = len * 2 - 1;
    uint8_t res[128] = {0};
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < len; ++j) {
            /* digit multiplication fits in 16 bits */
            uint8_t prod = dig[i] * dig[j];
            res[i + j] = (res[i + j] + prod) % 5;
        }
    }

    /* Convert the result back to an ordinary integer (decimal). */
    seq_t val = 0;
    for (int k = (int)res_len - 1; k >= 0; --k)
        val = val * 5 + res[(size_t)k];

    return val;
}
