/*
 * A088626  a(n) = 42*binomial(n,10).
 * https://oeis.org/A088626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42*binomial(n,10)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 33, u64 n <= 267,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 10, binomial(n,10) = 0 */
    if (n < 10) {
        return 0;
    }

    /* Compute binomial(n,10) multiplicatively */
    const idx_t k = 10;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by the i-th numerator term: n-k+i */
        res = res * (seq_t)(n - k + i);
        /* Exact division by the denominator term i */
        res = res / (seq_t)i;
    }

    /* Multiply by the constant factor 42 */
    res = res * (seq_t)42;

    return res;
}
