/*
 * A096036  a(n) = ceiling(n/a(ceiling(n/2))); a(1) = 1.
 * https://oeis.org/A096036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   _a_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_a_rec.h"
/* Recursive formula: a(n) = ceil( n / a( ceil(n/2) ) ) ,  a(1) = 1 */
/* The implementation is fully recursive, making one call per distinct
   argument.  All arithmetic is performed with the seq_t type, using only
   the allowed operators (+, -, *, /, %) and conversion from integer
   constants. */

extern seq_t a(idx_t n)
{
    return _a_rec(n);
}
