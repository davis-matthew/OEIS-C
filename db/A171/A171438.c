/*
 * A171438  a(n) = 0+1+2+...+n in lunar arithmetic in base 3 written in base
 *          10.
 * https://oeis.org/A171438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0+1+2+...+n in lunar arithmetic in base 3 written in base
 *            10
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Extract base‑3 digits of n (least significant first). */
    unsigned char digits[65];
    size_t len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = tmp % 3;   /* digit 0..2 */
        tmp /= 3;
    } while (tmp > 0);

    seq_t result = 0;
    bool higher_nonzero = false;  /* any more significant digit >0? */

    for (size_t i = 0; i < len; ++i) {
        unsigned d = digits[len - 1 - i];     /* most significant first */
        unsigned maxdig = (d == 2 || higher_nonzero) ? 2 : d;
        result = result * 3 + maxdig;
        if (d > 0) higher_nonzero = true;
    }

    return result;
}
