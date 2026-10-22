/*
 * A004262  a(n) = floor(100*log_2(n)).
 * https://oeis.org/A004262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(100*log_2(n)) = (number of binary digits of n^100)
 *            - 1, computed exactly with multiprecision integers
 * Range:     u8 n <= 5, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t limb[128];
    int len = 1, i, k;
    uint64_t top;
    long long bits;
    limb[0] = 1;
    for (k = 0; k < 100; k++) {
        unsigned __int128 carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 p = (unsigned __int128)limb[i] * (uint64_t)n + carry;
            limb[i] = (uint64_t)p;
            carry = p >> 64;
        }
        if (carry != 0) limb[len++] = (uint64_t)carry;
    }
    while (len > 1 && limb[len - 1] == 0) len--;
    top = limb[len - 1];
    bits = 64LL * (len - 1);
    while (top != 0) { bits++; top >>= 1; }
    return (seq_t)(uint64_t)(bits - 1);
}
