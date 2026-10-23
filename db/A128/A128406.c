/*
 * A128406  a(n) = (n+1)*2^(n*(n+1)).
 * https://oeis.org/A128406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1) * 2^(n*(n+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

/* Compute a(n) for OEIS sequence A128406.
   The implementation uses the helper functions pow2 and seq_mul 
   to avoid any overflow issues for arbitrary precision builds. */
seq_t a(idx_t n)
{
    /* exponent = n * (n + 1) fits in idx_t for the expected limits. */
    idx_t exp = n * (n + 1);

    /* 2^exp as a seq_t value */
    seq_t pow = pow2(exp);

    /* (n + 1) as seq_t */
    seq_t factor = (seq_t)(n + 1);

    /* Result = (n + 1) * 2^exp */
    return seq_mul(factor, pow);
}
