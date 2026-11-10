/*
 * A217485  Convolution of the numbers in sequence A080253.
 * https://oeis.org/A217485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} c(k)*c(n-k), where c(0) = 1, c(m) = 1 +
 *            Sum_{r=1..m} 2^r*binomial(m,r)*c(m-r) (A080253)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *c = mem_new(seq_t, N + 1);
    if (!c)
        abort();
    c[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        seq_t value = (seq_t)1;
        seq_t pow2r = (seq_t)2;
        seq_t comb = (seq_t)m;
        for (size_t r = 1; r <= m; ++r) {
            value += pow2r * comb * c[m - r];
            if (r < m) {
                comb = comb * (seq_t)(m - r) / (seq_t)(r + 1);
                pow2r <<= 1;
            }
        }
        c[m] = value;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k <= N; ++k)
        sum += c[k] * c[N - k];
    mem_free(c);
    return sum;
}
