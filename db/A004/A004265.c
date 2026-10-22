/*
 * A004265  a(n) = floor(1000*log_2(n)).
 * https://oeis.org/A004265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1000*log_2(n)) = floor(log_2(n^1000)) =
 *            bitlength(n^1000) - 1, computed exactly using a multi-precision
 *            integer for n^1000
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    size_t cap = 2 * 1000 + 4;
    uint32_t *d = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t len = 1;
    size_t i;
    int r;
    uint64_t bits;
    uint32_t top;
    d[0] = 1;
    for (r = 0; r < 1000; r++) {
        uint64_t carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 cur = (unsigned __int128)d[i] * (uint64_t)n + carry;
            d[i] = (uint32_t)cur;
            carry = (uint64_t)(cur >> 32);
        }
        while (carry) {
            d[len++] = (uint32_t)carry;
            carry >>= 32;
        }
    }
    top = d[len - 1];
    bits = (uint64_t)(len - 1) * 32;
    while (top) {
        bits++;
        top >>= 1;
    }
    free(d);
    return (seq_t)(bits - 1);
}
