/*
 * A157060  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 22.
 * https://oeis.org/A157060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1..min(n,11)} binomial(n+1, i) * binomial(10, i-1)
 *            * binomial(n-i+11, 11)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 35, u128 n <= 276,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 1: a(1) is first term */
    const idx_t K = 11;           /* k from the definition (sum of abs = 22) */
    seq_t result = 0;
    idx_t max_i = (n < K) ? n : K;   /* binomial(10, i-1) is zero for i>11 */
    for (idx_t i = 1; i <= max_i; ++i) {
        seq_t term = binomial(n + 1, i);         /* C(n+1 , i) */
        term = term * binomial(K - 1, i - 1);    /* C(10 , i-1) */
        term = term * binomial(n - i + K, K);    /* C(n-i+11 , 11) */
        result += term;
    }
    return result;
}
