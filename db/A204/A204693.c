/*
 * A204693  a(n) = n^n (mod 7).
 * https://oeis.org/A204693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n mod 7
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Special case for n = 0: 0^0 is defined as 1 in this sequence */
    if (n == 0)
        return (seq_t)1;

    /* Reduce base modulo 7 */
    seq_t base = (seq_t)(n % 7ULL);

    /* If the base is a multiple of 7, the result is 0 (for n > 0) */
    if (base == 0)
        return (seq_t)0;

    /* Reduce exponent modulo 6 using Fermat's little theorem:
       a^6 ≡ 1 (mod 7) for a not divisible by 7.
       Therefore a^n ≡ a^(n mod 6) (mod 7), with exponent 0 meaning 1. */
    idx_t exp = n % 6ULL;
    if (exp == 0)
        return (seq_t)1;

    /* Compute the modular exponentiation with the small modulus 7. */
    return mod_pow(base, exp, (seq_t)7);
}
