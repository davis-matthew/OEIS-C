/*
 * A013893  a(n) = 19^(5*n + 4).
 * https://oeis.org/A013893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19^(5*n + 4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)130321;            /* 19^4 = 130321 */
    seq_t r = seq_pow((seq_t)2476099, n);  /* 2476099 = 19^5 */
    r = seq_mul(r, (seq_t)130321);        /* multiply by 19^4 */
    return r;
}
