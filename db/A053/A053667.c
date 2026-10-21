/*
 * A053667  Product of digits of n^2.
 * https://oeis.org/A053667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A007954(n^2) – product of decimal digits of n²
 * Range:     u8 n <= 15, u16 n <= 299, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n² in seq_t type */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Edge case: n = 0 => a(0) = 0 */
    if (sq == 0)
        return 0;

    seq_t prod = 1;
    while (sq > 0) {
        seq_t digit = sq % 10;   /* last decimal digit */
        prod *= digit;
        sq /= 10;
    }
    return prod;
}
