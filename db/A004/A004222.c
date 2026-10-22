/*
 * A004222  a(n) = 100*log_10(n) rounded down.
 * https://oeis.org/A004222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(100*log_10(n)) = (number of decimal digits of
 *            n^100) - 1, computed exactly with base-10^9 big integers
 * Range:     u8 n <= 363, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint32_t limbs[260];
    int len = 1, i, j, digits;
    uint32_t top;
    limbs[0] = 1;
    for (i = 0; i < 100; i++) {
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
    digits = 9 * (len - 1);
    top = limbs[len - 1];
    while (top > 0) {
        digits++;
        top /= 10;
    }
    return (seq_t)(long long)(digits - 1);
}
