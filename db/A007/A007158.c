/*
 * A007158  Essentially n-ary operations in a certain 3-element algebra.
 * https://oeis.org/A007158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} (-1)^(n-i) * binomial(n, i) * A007155(i),
 *            where A007155(i) = 2^(2^i) * 3^(3^i - 2^i)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static seq_t pw_seq(seq_t b, uint64_t e)
{
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

static seq_t a7155(idx_t i)
{
    uint64_t p2 = 1, p3 = 1;
    idx_t k;
    for (k = 0; k < i; k++) { p2 *= 2; p3 *= 3; }
    return pw_seq((seq_t)2, p2) * pw_seq((seq_t)3, p3 - p2);
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t i;
    for (i = 0; i <= n; i++) {
        seq_t t = binom(n, i) * a7155(i);
        if (((n - i) & 1) == 0) pos += t;
        else neg += t;
    }
    return pos - neg;
}
