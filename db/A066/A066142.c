/*
 * A066142  a(n) = (n!)^2 + n! + 1.
 * https://oeis.org/A066142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n!)^2 + n! + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);   /* compute n! */
    return f * f + f + 1;     /* (n!)^2 + n! + 1 */
}
