/*
 * A171679  Squares in lunar arithmetic in base 9 written in base 10.
 * https://oeis.org/A171679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal value of the lunar square of n in base 9
 *            (i.e., (n * n) in lunar arithmetic with base 9)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑9 digits (least significant first) */
    uint8_t digs[64];
    idx_t len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digs[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digs[len++] = tmp % 9;
            tmp /= 9;
        }
    }

    /* Resulting digits of the lunar square (size 2*len) */
    uint8_t res[128];
    idx_t res_len = 2 * len;
    for (idx_t i = 0; i < res_len; ++i)
        res[i] = 0;

    /* Lunar multiplication: each pair of digits contributes
       min(a_i, a_j) at position i+j, taking the maximum over all pairs. */
    for (idx_t i = 0; i < len; ++i) {
        uint8_t di = digs[i];
        for (idx_t j = 0; j < len; ++j) {
            uint8_t dj = digs[j];
            uint8_t md = (di < dj) ? di : dj;
            idx_t pos = i + j;
            if (md > res[pos])
                res[pos] = md;
        }
    }

    /* Convert the result digits back to a decimal number (base‑10) */
    seq_t value = 0;
    for (idx_t i = res_len; i-- > 0; )
        value = value * 9 + res[i];

    return value;
}
