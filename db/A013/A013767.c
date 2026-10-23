/*
 * A013767  a(n) = 20^(3*n + 2).
 * https://oeis.org/A013767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20^(3*n + 2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t exp = 3 * n + 2;            // exponent 3*n+2
    seq_t base = (seq_t)20;           // base 20
    return seq_pow(base, exp);        // compute 20^(3*n+2)
}
