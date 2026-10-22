/*
 * A004223  a(n) = 100*log_10(n) rounded to the nearest integer.
 * https://oeis.org/A004223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(100*log_10(n)) = floor((f+1)/2), where f =
 *            floor(log_10(n^200)) = (number of decimal digits of n^200) - 1;
 *            n^200 is computed exactly in base 10^9
 * Range:     u8 n <= 358, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint32_t limbs[450];
    int len = 1, i, j;
    uint64_t top, f, k;
    limbs[0] = 1;
    for (i = 0; i < 200; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; j < len; j++) {
            unsigned __int128 cur = (unsigned __int128)limbs[j] * n + carry;
            limbs[j] = (uint32_t)(cur % 1000000000u);
            carry = cur / 1000000000u;
        }
        while (carry > 0) {
            limbs[len++] = (uint32_t)(carry % 1000000000u);
            carry /= 1000000000u;
        }
    }
    /* number of decimal digits */
    top = limbs[len - 1];
    f = (uint64_t)(len - 1) * 9;
    while (top > 0) { f++; top /= 10; }
    f -= 1; /* floor(log10(n^200)) */
    k = (f + 1) / 2;
    return (seq_t)k;
}
