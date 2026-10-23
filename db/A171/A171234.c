/*
 * A171234  Squares in lunar arithmetic in base 3 written in base 10.
 * https://oeis.org/A171234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = lunar square of n in base 3 (addition = max,
 *            multiplication = min) a(n) is the decimal value of n ⊗ n
 *            computed under lunar arithmetic in base 3
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* extract base‑3 digits of n (least significant first) */
    uint8_t digits[64];
    idx_t t = n;
    int d = 0;
    if (t == 0) {
        digits[0] = 0;
        d = 1;
    } else {
        while (t > 0) {
            digits[d++] = (uint8_t)(t % 3);
            t /= 3;
        }
    }

    /* compute the lunar square digits */
    int prod_len = 2 * d - 1;
    uint8_t prod[128];
    for (int i = 0; i < prod_len; ++i) prod[i] = 0;

    for (int i = 0; i < d; ++i) {
        uint8_t di = digits[i];
        for (int j = 0; j < d; ++j) {
            uint8_t dj = digits[j];
            uint8_t mn = di < dj ? di : dj;
            int pos = i + j;
            if (mn > prod[pos]) prod[pos] = mn;
        }
    }

    /* convert the result digits back to a decimal seq_t value */
    seq_t result = 0;
    for (int k = prod_len - 1; k >= 0; --k) {
        result = result * 3 + prod[k];
    }
    return result;
}
