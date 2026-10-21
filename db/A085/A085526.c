/*
 * A085526  a(n) = n^(2n+1).
 * https://oeis.org/A085526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    /* Exponent is 2*n+1, base is n converted to seq_t */
    return _pow_seq_t((seq_t)n, (idx_t)(n * 2 + 1));
}
