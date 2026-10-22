/*
 * A006995  Binary palindromes: numbers whose binary expansion is palindromic.
 * https://oeis.org/A006995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice, hear
 * Formula:   direct construction: for n >= 2, take m = n-1 and find the bit
 *            length L (there are 2^floor((L-1)/2) binary palindromes of
 *            length L); the k-th (0-based) one has upper half 2^(ceil(L/2)-1)
 *            + k, mirrored into its lower bits; a(1) = 0
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t m = (uint64_t)(n - 1);
    unsigned L = 1;
    for (;;) {
        uint64_t cnt = (uint64_t)1 << ((L - 1) / 2);
        if (m <= cnt) break;
        m -= cnt;
        L++;
    }
    uint64_t k = m - 1;
    unsigned h = (L + 1) / 2;
    uint64_t half = ((uint64_t)1 << (h - 1)) + k;
    seq_t v = (seq_t)half;
    uint64_t t = half;
    if (L & 1) t >>= 1;
    unsigned i;
    for (i = 0; i < L - h; i++) {
        v = (v << 1) | (seq_t)(t & 1);
        t >>= 1;
    }
    return v;
}
