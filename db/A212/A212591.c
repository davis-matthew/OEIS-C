/*
 * A212591  a(n) is the smallest value of k for which A020986(k) = n.
 * https://oeis.org/A212591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k with A020986(k)=n. The sequence satisfies
 *            a(1)=0 and for n>=2 a(n)-a(n-1)= (2^(2*v2(t)+1)+1)/3 where t =
 *            (n%2==0 ? n/2 : (n-1)/2) and v2(t) is the exponent of 2 in t
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline int v2_idx(idx_t x)
{
    int k = 0;
    while ((x & 1ULL) == 0ULL) {
        x >>= 1;
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t t = (i & 1) ? (i - 1) / 2 : i / 2;
        int w = v2_idx(t);
        seq_t d = (pow2((idx_t)(2 * w + 1)) + (seq_t)1) / (seq_t)3;
        sum += d;
    }
    return sum;
}
