/*
 * A127229  a(n) = numerator of ((n + 5)! - (n - 5)!)/n!.
 * https://oeis.org/A127229
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (n+5)!/(n-5)! - 1
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 83, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 5. For n < 5 the value is not specified,
       but we return 0 to avoid underflow in unsigned arithmetic. */
    if (n < 5)
        return (seq_t)0;

    /* Compute the product of the ten consecutive integers from n-4 to n+5. */
    seq_t prod = 1;
    seq_t start = (seq_t)n - (seq_t)4;
    seq_t end   = (seq_t)n + (seq_t)5;

    for (seq_t k = start; k <= end; ++k)
        prod *= k;

    return prod - (seq_t)1;
}
