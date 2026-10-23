/*
 * A137850  a(n) = n base 5, under morphism f(1) = 121, f(2) = 123, f(3) =
 *          141, f(4) = 142, or 0 if n base 5 has a zero.
 * https://oeis.org/A137850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   replace each base‑5 digit d (1‥4) of n by 121, 123, 141, 142
 *            respectively; if any digit is 0 the value is 0
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* collect base‑5 digits (least significant first) */
    uint8_t digs[20];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digs[len++] = t % 5;
        t /= 5;
    }
    /* if any digit is 0, result is 0 */
    for (size_t i = 0; i < len; ++i) {
        if (digs[i] == 0) {
            return (seq_t)0;
        }
    }
    /* mapping from digit to 3‑digit code */
    const seq_t mapping[5] = {0, (seq_t)121, (seq_t)123, (seq_t)141, (seq_t)142};

    /* build the decimal integer by concatenating the codes */
    seq_t res = 0;
    for (size_t j = 0; j < len; ++j) {
        uint8_t d = digs[len - 1 - j];   /* most significant digit first */
        res = res * 1000 + mapping[d];
    }
    return res;
}
