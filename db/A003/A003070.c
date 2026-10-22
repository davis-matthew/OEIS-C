/*
 * A003070  a(n) = ceiling(log_2 n!).
 * https://oeis.org/A003070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(log_2 n!); computed exactly as the bit length of
 *            n! (for n >= 3, n! is not a power of 2, so ceil(log2 n!) =
 *            bitlength(n!)), a(0) = a(1) = 0, a(2) = 1
 * Range:     u8 n <= 57, u16 n <= 5910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    unsigned nb = 0;
    idx_t t = n;
    while (t) { nb++; t >>= 1; }
    size_t cap = (size_t)((n * (idx_t)nb) / 32 + 4);
    uint32_t *x = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t len = 1;
    x[0] = 1;
    uint64_t twos = 0;

    for (idx_t k = 3; k <= n; k++) {
        uint64_t m = (uint64_t)k;
        while ((m & 1u) == 0) { m >>= 1; twos++; }
        if (m == 1) continue;
        uint64_t carry = 0;
        for (size_t i = 0; i < len; i++) {
            uint64_t p = (uint64_t)x[i] * m + carry;
            x[i] = (uint32_t)p;
            carry = p >> 32;
        }
        while (carry) {
            x[len++] = (uint32_t)carry;
            carry >>= 32;
        }
    }
    /* factor 2 from k = 2 */
    twos++;

    uint32_t top = x[len - 1];
    unsigned tb = 0;
    while (top) { tb++; top >>= 1; }
    uint64_t bits = (uint64_t)(len - 1) * 32 + tb + twos;
    free(x);
    return (seq_t)bits;
}
