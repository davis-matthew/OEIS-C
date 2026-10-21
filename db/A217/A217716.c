/*
 * A217716  Duplicate of A129824.
 * https://oeis.org/A217716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Product_{k=0..n} (binomial(n,k) + 1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(n, k);
        term += 1;
        prod *= term;
    }
    return prod;
}
