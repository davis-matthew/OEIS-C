/*
 * A065275  Infinite binary tree inspired permutation of N: 1 -> 3, 11ab..yz
 *          -> 11ab..yz0, 10ab..y0 -> 10ab..y, 10ab..y1 -> 11ab..y1.
 * https://oeis.org/A065275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n==1; * = 2*n if n has top two bits 11; * = n/2 if n
 *            even and top bits 10; * = n + 2^(len(n)-2) if n odd and top bits
 *            10
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int bitlen(idx_t x) {
    int l = 0;
    while (x) {
        x >>= 1;
        l++;
    }
    return l;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    int len = bitlen(n);
    /* second most significant bit */
    int second = (int)((n >> (len - 2)) & 1ULL);
    if (second) {
        return (seq_t)n << 1;
    } else {
        if ((n & 1ULL) == 0) {
            return (seq_t)(n >> 1);
        } else {
            return (seq_t)n + (seq_t)(1ULL << (len - 2));
        }
    }
}
