/*
 * A053506  a(n) = (n-1)*n^(n-2).
 * https://oeis.org/A053506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * n^(n-2), offset 1 (a(1) = 0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n = 1 the exponent n-2 is -1; return 0 directly. */
    if (n <= 1) return (seq_t)0;

    /* Compute n^(n-2) using the utility power routine. */
    seq_t pow = pow_uint((uint64_t)n, n - 2);

    /* Multiply by (n-1). The cast ensures the multiplication
       is performed in the seq_t type. */
    return ((seq_t)(n - 1)) * pow;
}
