/*
 * A169821  a(n) = n*n in carryless arithmetic mod 9 in base 10.
 * https://oeis.org/A169821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*n in carryless arithmetic mod 9 in base 10
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return 0;

    /* Extract decimal digits of n (least‑significant first) */
    uint8_t d[21];          /* 64‑bit n < 10^20, so 20 digits max */
    int dn = 0;
    uint64_t t = n;
    if (t == 0) {
        d[0] = 0;
        dn = 1;
    } else {
        while (t != 0) {
            d[dn++] = (uint8_t)(t % 10);
            t /= 10;
        }
    }

    /* Result digits (carryless convolution). Size at most 2*dn */
    uint8_t r[41] = {0};
    int rn = 2 * dn;

    for (int i = 0; i < dn; ++i) {
        for (int j = 0; j < dn; ++j) {
            int sum = r[i + j] + d[i] * d[j];
            /* Addition modulo 9 */
            r[i + j] = (uint8_t)(sum % 9);
        }
    }

    /* Find most significant non‑zero digit */
    int ms = rn - 1;
    while (ms > 0 && r[ms] == 0)
        --ms;

    /* Convert digit array back to the integer value */
    seq_t result = 0;
    for (int k = ms; k >= 0; --k) {
        result = result * 10 + r[k];
    }
    return result;
}
