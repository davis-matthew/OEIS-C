/*
 * A157061  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 24.
 * https://oeis.org/A157061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..min(n,12)} C(n+1,i) * C(11,i-1) * C(n-i+12,12)
 *            where C(a,b) is the binomial coefficient
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 31, u128 n <= 212,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* sequence is indexed from 1 */
    idx_t max_i = n < 12 ? n : 12;
    seq_t result = 0;
    for (idx_t i = 1; i <= max_i; ++i) {
        seq_t term = binomial(n + 1, i);
        term *= binomial(11, i - 1);
        term *= binomial(n - i + 12, 12);
        result += term;
    }
    return result;
}
