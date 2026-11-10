/*
 * A090451  Alternating row sums of triangle A090447.
 * https://oeis.org/A090451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{m=0..n} (-1)^m * prod_{k=0..m} binomial(n,k) (row n
 *            of A090447, alternating sum)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t c = 1;      /* binomial(n,k) */
    seq_t prod = 1;   /* partial product of binomials */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        prod = prod * c;
        if (k & 1) sum -= prod; else sum += prod;
        c = c * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return sum;
}
