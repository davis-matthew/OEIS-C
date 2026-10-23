/*
 * A158879  a(n) = 4^n + n.
 * https://oeis.org/A158879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 4^n using the utility big_pow, then adds n. */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)4;            /* base = 4 */
    seq_t exp    = (seq_t)n;            /* exponent = n */
    seq_t power  = big_pow(base, exp);  /* power = 4^n */
    seq_t result = power + exp;         /* result = 4^n + n */
    return result;
}
