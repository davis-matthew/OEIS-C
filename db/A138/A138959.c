/*
 * A138959  a(n) = 8^n mod 5^n.
 * https://oeis.org/A138959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n mod 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   mod_pow, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Compute modulus 5^n */
    seq_t mod = seq_pow((seq_t)5, n);

    /* Compute 8^n mod 5^n */
    seq_t result = mod_pow((seq_t)8, n, mod);

    return result;
}
