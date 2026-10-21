/*
 * A239131  A sequence with period length 54; the companion of x(n) =
 *          A239130(n), the smallest positive solution of 3^4*x - 2^n*y = 1
 *          for n >= 0.
 * https://oeis.org/A239131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41^(n+27) (mod 81), period = 54
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* use the generic modular exponentiation helper from the utils */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* reduce exponent modulo the period (phi(81) = 54) */
    idx_t e = (n % 54ULL);        /* n mod 54 */
    e = (e + 27ULL) % 54ULL;      /* (n+27) mod 54 */

    /* base = 41, modulus = 81 */
    seq_t base = (seq_t)41;
    seq_t mod  = (seq_t)81;

    return mod_pow(base, e, mod);
}
