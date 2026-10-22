/*
 * A004267  a(n) = ceiling(1000*log_2(n)).
 * https://oeis.org/A004267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(1000*log_2(n)) = least k with 2^k >= n^1000,
 *            found exactly from the bit length of n^1000 computed with
 *            multiword integers
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    uint64_t bits = 0, t = n, len, i, j, used, top, bitlen, k;
    uint32_t *w;
    int pow2 = (n & (n - 1)) == 0;
    while (t) { bits++; t >>= 1; }
    len = (bits * 1000) / 32 + 4;
    w = (uint32_t *)malloc(len * sizeof(uint32_t));
    w[0] = 1;
    used = 1;
    for (i = 0; i < 1000; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; j < used; j++) {
            unsigned __int128 p = (unsigned __int128)w[j] * n + carry;
            w[j] = (uint32_t)p;
            carry = p >> 32;
        }
        while (carry) {
            w[used++] = (uint32_t)carry;
            carry >>= 32;
        }
    }
    top = w[used - 1];
    bitlen = (used - 1) * 32;
    while (top) { bitlen++; top >>= 1; }
    free(w);
    k = pow2 ? bitlen - 1 : bitlen;
    return (seq_t)k;
}
