/*
 * A166725  a(n) = (2*n+1)*25^n.
 * https://oeis.org/A166725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*25^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pow25 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow25 *= (seq_t)25;
    }
    seq_t mult = ((seq_t)n * (seq_t)2) + (seq_t)1;
    return pow25 * mult;
}
