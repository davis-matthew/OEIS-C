/*
 * A097788  a(n)=4a(n-1)+C(n+3,3),n>0, a(0)=1.
 * https://oeis.org/A097788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + binomial(n+3,3), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t bin = binomial(i + 3, (idx_t)3); /* C(i+3,3) */
        val = val * (seq_t)4 + bin;
    }
    return val;
}
