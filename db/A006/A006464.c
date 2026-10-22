/*
 * A006464  Continued fraction for Sum_{n>=0} 1/4^(2^n).
 * https://oeis.org/A006464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 0 and a(1..) is the limit of the words s_k, where s_2 =
 *            [3,6,4,3] and s_{k+1} = [s_k(1..L-1), s_k(L)+1, s_k(L)-1,
 *            s_k(L-1), ..., s_k(1)] with L = |s_k| = 2^k (the folding lemma
 *            applied to adding 1/4^(2^(k+1))). Because s_k(L) = 3, this
 *            gives: for 1 <= i <= 4, a(i) = 3,6,4,3; otherwise take...
 * Range:     u8 n <= 9007, u16 n <= 9007, u32 n <= 9007, u64 n <= 9007,
 *            u128 n <= 9007, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const int base[5] = {0, 3, 6, 4, 3};
    uint64_t i = (uint64_t)n;
    if (i == 0) return (seq_t)0;
    for (;;) {
        if (i < 4) return (seq_t)base[i];
        /* smallest power of two L >= 8 with i < L, so i lies in the stable part of s */
        uint64_t L = 8;
        while (L <= i && L < ((uint64_t)1 << 63)) L <<= 1;
        uint64_t h = L >> 1;
        if (i == h) return (seq_t)4;
        if (i == h + 1) return (seq_t)2;
        if (i < h) {
            /* this cannot happen, because L is the smallest power of two above i */
            L = h;
            continue;
        }
        i = L + 1 - i;
    }
}
