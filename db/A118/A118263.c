/*
 * A118263  a(3n) = 2^n, a(3n+1) = 3^n, a(3n+2) = 4^n.
 * https://oeis.org/A118263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = 2^n, a(3n+1) = 3^n, a(3n+2) = 4^n
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 49, u64 n <= 97, u128 n <= 193,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Determine the remainder and quotient */
    const idx_t r = n % 3;
    const idx_t k = n / 3;

    /* Choose the base according to the remainder */
    const seq_t base = (r == 0) ? (seq_t)2
                     : (r == 1) ? (seq_t)3
                     : (seq_t)4;

    /* Compute the power using the provided big_pow routine */
    return big_pow(base, (seq_t)k);
}
