/*
 * A191562  a(n) = 7^n mod 3*n.
 * https://oeis.org/A191562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod (3*n)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Compute modulus 3*n */
    seq_t mod = (seq_t)3 * (seq_t)n;
    /* Compute 7^n modulo mod */
    return mod_pow((seq_t)7, n, mod);
}
