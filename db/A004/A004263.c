/*
 * A004263  a(n) = round(100*log_2(n)).
 * https://oeis.org/A004263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(100*log_2(n)), computed exactly: a(n) = floor(b/2),
 *            where b = number of binary digits of n^200 (since a(n) = largest
 *            k with 2^(2k-1) <= n^200)
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t limbs[210];
    int len = 1, i, r;
    uint64_t top, bits;
    limbs[0] = 1;
    for (r = 0; r < 200; r++) {
        uint64_t carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 p = (unsigned __int128)limbs[i] * (uint64_t)n + carry;
            limbs[i] = (uint64_t)p;
            carry = (uint64_t)(p >> 64);
        }
        if (carry) limbs[len++] = carry;
    }
    top = limbs[len - 1];
    bits = (uint64_t)(len - 1) * 64;
    while (top) {
        bits++;
        top >>= 1;
    }
    return (seq_t)(uint64_t)(bits / 2);
}
