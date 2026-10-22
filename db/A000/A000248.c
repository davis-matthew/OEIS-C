/*
 * A000248  Expansion of e.g.f. exp(x*exp(x)).
 * https://oeis.org/A000248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*(n-k)^k
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000248_pow.h"

seq_t a(idx_t n)
{
    seq_t s = 0;
    seq_t c = 1;
    idx_t k;
    for (k = 0; k <= n; k++) {
        if (k > 0) c = c * (seq_t)(n - k + 1) / (seq_t)k;
        s += c * a000248_pow((seq_t)(n - k), k);
    }
    return s;
}
