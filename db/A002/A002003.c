/*
 * A002003  a(n) = 2 * Sum_{k=0..n-1} binomial(n-1, k)*binomial(n+k, k).
 * https://oeis.org/A002003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=0..n-1} binomial(n-1, k)*binomial(n+k, k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 953 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k < n; k++) {
        seq_t term1 = binomial(n - 1, k);
        seq_t term2 = binomial(n + k, k);
        sum += term1 * term2;
    }
    return 2 * sum;
}
