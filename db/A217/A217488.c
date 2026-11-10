/*
 * A217488  Alternating sums of the squares of the numbers in sequence
 *          A080253.
 * https://oeis.org/A217488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*c(k)^2, c(n) = A080253(n); i.e.
 *            a(n) = c(n)^2 - a(n-1), a(0) = 1, with c(0) = 1, c(n) = 1 +
 *            Sum_{r=1..n} 2^r*binomial(n,r)*c(n-r)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t *c = mem_new(seq_t, (size_t)n + 1);
    if (!c)
        abort();
    c[0] = (seq_t)1;
    seq_t acc = (seq_t)1;                 /* a(0) = c(0)^2 */
    for (idx_t idx = 1; idx <= n; ++idx) {
        seq_t value = (seq_t)1;
        seq_t pow2r = (seq_t)2;
        seq_t comb = (seq_t)idx;
        for (idx_t r = 1; r <= idx; ++r) {
            value += pow2r * comb * c[idx - r];
            if (r < idx) {
                comb = comb * (seq_t)(idx - r) / (seq_t)(r + 1);
                pow2r <<= 1;
            }
        }
        c[idx] = value;
        acc = value * value - acc;
    }
    mem_free(c);
    return acc;
}
