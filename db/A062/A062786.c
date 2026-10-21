/*
 * A062786  Centered 10-gonal numbers.
 * https://oeis.org/A062786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*n*(n-1) + 1
 * Range:     u8 n <= 7, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is unsigned; n >= 1 */
    return ((seq_t)5) * (seq_t)n * (seq_t)(n - 1) + (seq_t)1;
}
