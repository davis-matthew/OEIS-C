/*
 * A004261  a(n) = ceiling(10*log_2(n)).
 * https://oeis.org/A004261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(10*log_2(n)) = smallest k with 2^k >= n^10,
 *            computed exactly as the bit length of n^10 - 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t limb[12];
    int len = 1, i, j, bits;
    if (n <= 1) return (seq_t)0;
    limb[0] = 1;
    for (j = 0; j < 10; j++) {
        unsigned __int128 carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 t = (unsigned __int128)limb[i] * n + carry;
            limb[i] = (uint64_t)t;
            carry = t >> 64;
        }
        if (carry) limb[len++] = (uint64_t)carry;
    }
    /* subtract 1 */
    for (i = 0; i < len; i++) {
        if (limb[i] != 0) { limb[i]--; break; }
        limb[i] = UINT64_MAX;
    }
    while (len > 0 && limb[len - 1] == 0) len--;
    if (len == 0) return (seq_t)0;
    bits = (len - 1) * 64;
    {
        uint64_t top = limb[len - 1];
        while (top) { bits++; top >>= 1; }
    }
    return (seq_t)bits;
}
