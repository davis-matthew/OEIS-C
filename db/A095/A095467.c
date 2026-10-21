/*
 * A095467  a(n) = 37 written in base 12 - n.
 * https://oeis.org/A095467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of decimal 37 in base (12-n)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    const idx_t NUM = 37;
    /* The sequence is defined for n = 0 … 11 (base = 12‑n ≥ 1). */
    if (n >= 12) return 0;

    idx_t base = 12 - n;

    /* Unary (base = 1) representation: 37 ones. */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < NUM; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Build the base‑`base` digits of 37. */
    uint8_t digits[64];
    idx_t len = 0;
    idx_t tmp = NUM;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % base);
        tmp /= base;
    }

    /* Convert the digit vector to a decimal integer. */
    seq_t res = 0;
    for (idx_t i = 0; i < len; ++i) {
        res = res * 10 + digits[len - 1 - i];
    }
    return res;
}
