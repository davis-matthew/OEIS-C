/*
 * A233471  a(n) = 3^n mod n^2.
 * https://oeis.org/A233471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod n^2
 * Range:     u8 n <= 15, u16 n <= 276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for the official offset */
    seq_t mod = (seq_t)n * (seq_t)n;/* n^2 as the modulus */
    return mod_pow((seq_t)3, n, mod);
}
