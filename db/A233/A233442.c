/*
 * A233442  a(n) = 2^n mod n^3.
 * https://oeis.org/A233442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n mod n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1635, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Compute n^3 */
    seq_t nn  = (seq_t)n;
    seq_t mod = nn * nn * nn;

    /* Return 2^n (mod n^3) using the provided modular exponentiation routine */
    return mod_pow((seq_t)2, n, mod);
}
