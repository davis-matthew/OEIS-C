/*
 * A004260  a(n) = round(10*log_2(n)).
 * https://oeis.org/A004260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(10*log_2(n)) = floor((L+1)/2), where L =
 *            floor(log_2(n^20)), computed exactly in integers
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t limb[24];
    int len = 1, i, j, top;
    uint64_t L, k, w;
    limb[0] = 1;
    for (j = 0; j < 20; j++) {
        unsigned __int128 carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 t = (unsigned __int128)limb[i] * (uint64_t)n + carry;
            limb[i] = (uint64_t)t;
            carry = t >> 64;
        }
        if (carry != 0) limb[len++] = (uint64_t)carry;
    }
    top = len - 1;
    while (top > 0 && limb[top] == 0) top--;
    w = limb[top];
    L = 64 * (uint64_t)top;
    while (w > 1) { w >>= 1; L++; }
    k = (L + 1) / 2;
    return (seq_t)k;
}
