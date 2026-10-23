/*
 * A170989  Carryless product n X n in base 8.
 * https://oeis.org/A170989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = carryless product of n with itself in base 8 * * The
 *            carryless product is defined digitwise modulo the base: * a(n) =
 *            ⊗_8 (n,n) * where each digit of the product is the sum of
 *            pairwise products of the * corresponding digits of the operands
 *            modulo 8, and no carries are * propagated from low...
 * Range:     u8 n <= 15, u16 n <= 511, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract base‑8 digits of n (least significant first). */
    uint8_t digits[64];
    size_t d = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[d++] = (uint8_t)(tmp & 7);  /* tmp % 8 */
        tmp >>= 3;                        /* tmp / 8 */
    }
    if (d == 0)                        /* n == 0 */
        return 0;

    /* Convolution modulo 8.  Result has 2*d‑1 digits. */
    uint8_t res[128] = {0};
    size_t reslen = 2 * d - 1;
    for (size_t i = 0; i < d; ++i) {
        for (size_t j = 0; j < d; ++j) {
            uint8_t prod = (digits[i] * digits[j]) & 7;  /* product mod 8 */
            uint8_t sum  = res[i + j] + prod;
            res[i + j]   = sum & 7;                      /* sum mod 8 */
        }
    }

    /* Convert digit vector back to a base‑8 integer. */
    seq_t result  = 0;
    seq_t mult    = 1;          /* 8⁰, 8¹, … */
    for (size_t k = 0; k < reslen; ++k) {
        result += (seq_t)res[k] * mult;
        mult   *= 8;
    }
    return result;
}
