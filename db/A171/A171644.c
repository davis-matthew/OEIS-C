/*
 * A171644  Squares in lunar arithmetic in base 8 written in base 8.
 * https://oeis.org/A171644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the lunar square of n in base 8 and return the decimal integer
   whose decimal digits are the base‑8 digits of the product. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑8 digits (least‑significant first). */
    uint8_t dig[64];
    size_t len = 0;

    if (n == 0) {
        dig[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            dig[len++] = tmp % 8;
            tmp /= 8;
        }
    }

    /* Compute lunar product: res[m] = max(min(dig[i],dig[j])) for i+j=m. */
    size_t res_len = 2 * len;
    uint8_t res[128] = {0};

    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < len; ++j) {
            size_t m = i + j;
            uint8_t mn = dig[i] < dig[j] ? dig[i] : dig[j];
            if (mn > res[m]) res[m] = mn;
        }
    }

    /* Strip any leading zero digits. */
    while (res_len > 1 && res[res_len - 1] == 0) {
        --res_len;
    }

    /* Pack the result into a decimal integer whose digits equal the base‑8 digits. */
    seq_t val = 0;
    for (size_t k = res_len; k > 0; --k) {
        val = val * (seq_t)10 + (seq_t)res[k - 1];
    }

    return val;
}
