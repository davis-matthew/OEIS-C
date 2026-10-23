/*
 * A169919  a(n) = n*n in the arithmetic where digits are added in base 10 (as
 *          usual) but when digits are to be multiplied they are also added in
 *          base 10.
 * https://oeis.org/A169919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*n computed with digit-sum multiplication In this
 *            arithmetic the usual product of two digits a*b is replaced by
 *            a+b, while all other operations (addition, carry, etc.) remain
 *            ordinary base‑10
 * Range:     u8 n <= 11, u16 n <= 295, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Extract decimal digits (least significant first) */
    uint8_t digits[25];               // 25 digits is enough for 64‑bit numbers
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = t % 10;
        t /= 10;
    }

    /* Precompute powers of 10 up to 2*len-1 */
    seq_t pow10[50];
    pow10[0] = 1;
    for (size_t k = 1; k <= 2 * len; ++k)
        pow10[k] = pow10[k - 1] * 10;

    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < len; ++j) {
            uint8_t s = digits[i] + digits[j];
            res += (seq_t)s * pow10[i + j];
        }
    }
    return res;
}
