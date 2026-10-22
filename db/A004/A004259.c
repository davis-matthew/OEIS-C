/*
 * A004259  a(n) = floor(10*log_2(n)).
 * https://oeis.org/A004259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(10*log_2(n)) = (number of bits of n^10) - 1,
 *            computed exactly with multiprecision integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t limbs[12];
    int len = 1, i, k, top;
    limbs[0] = 1;
    for (k = 0; k < 10; k++) {
        unsigned __int128 carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 t = (unsigned __int128)limbs[i] * (uint64_t)n + carry;
            limbs[i] = (uint64_t)t;
            carry = t >> 64;
        }
        if (carry) limbs[len++] = (uint64_t)carry;
    }
    while (len > 1 && limbs[len - 1] == 0) len--;
    top = 0;
    {
        uint64_t x = limbs[len - 1];
        while (x > 1) { x >>= 1; top++; }
    }
    return (seq_t)(long long)(64 * (len - 1) + top);
}
