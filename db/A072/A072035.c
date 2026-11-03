/*
 * A072035  Sum(binomial(n,k)*n^k*k^n,k=1..n).
 * https://oeis.org/A072035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{n} binomial(n,k) * n^k * k^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* binomial(n,k) */
        seq_t bin = binomial(n, k);

        /* n^k and k^n */
        seq_t npow = ipow((seq_t)n, (seq_t)k);
        seq_t kpow = ipow((seq_t)k, (seq_t)n);

        /* product */
        seq_t term = bin * npow * kpow;

        sum += term;
    }
    return sum;
}
