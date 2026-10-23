/*
 * A126431  a(n) = n*10^n.
 * https://oeis.org/A126431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*10^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function pow10(e) to compute 10^e as a seq_t value. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    return (seq_t)n * pow10(n);
}
