/*
 * A070727  n^7 mod 47.
 * https://oeis.org/A070727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 47
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   mul_mod_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_mod_small.h"

seq_t a(idx_t n)
{
    /* keep only the remainder of n modulo 47 before any multiplication */
    seq_t base = (seq_t)(n % 47);
    seq_t r = 1;                 /* (n^0) mod 47 = 1 */
    for (int i = 0; i < 7; ++i)
        r = mul_mod_small(r, base);   /* r = (r * base) mod 47  */
    return r;                /* r = n^7 mod 47 */
}
