/*
 * A174736  Number of nodes with index 2^2k in a dyadic tree build
 *          alternatively with the schema "l" between the index 2^2k and
 *          2^(2k+1) - 1, and the schema "^" between the index 2^(2k+1) and
 *          2^(2k+2) - 1.
 * https://oeis.org/A174736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((4^n + 2)/6) (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"  /* pow_uint(uint64_t base, idx_t exp)  */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp)     */

seq_t a(idx_t n)
{
    /* Compute 4^n as an arbitrary-precision integer. */
    seq_t pow4 = pow_uint((uint64_t)4, n);

    /* Compute the exponent e = (4^n + 2) / 6. */
    seq_t e = (pow4 + (seq_t)2) / (seq_t)6;

    /* Return 2^e. */
    return big_pow((seq_t)2, e);
}
