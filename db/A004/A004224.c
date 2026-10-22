/*
 * A004224  a(n) = 100*log_10(n) rounded up.
 * https://oeis.org/A004224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(100*log_10(n)); computed exactly as the number of
 *            decimal digits of n^100, minus 1 when n is a power of 10
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* n^100 in base 10^9 limbs, little-endian */
    uint32_t limbs[260];
    int len = 1, i, k;
    long long digits;
    uint32_t top;
    uint64_t m;
    int pow10 = 1;

    limbs[0] = 1;
    for (k = 0; k < 100; k++) {
        unsigned __int128 carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 cur = (unsigned __int128)limbs[i] * n + carry;
            limbs[i] = (uint32_t)(cur % 1000000000u);
            carry = cur / 1000000000u;
        }
        while (carry > 0) {
            limbs[len++] = (uint32_t)(carry % 1000000000u);
            carry /= 1000000000u;
        }
    }

    digits = 9LL * (len - 1);
    top = limbs[len - 1];
    while (top > 0) {
        digits++;
        top /= 10;
    }

    m = n;
    while (m >= 10 && m % 10 == 0) m /= 10;
    if (m != 1) pow10 = 0;

    if (pow10) digits -= 1;
    return (seq_t)digits;
}
