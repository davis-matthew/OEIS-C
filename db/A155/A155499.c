/*
 * A155499  a(n) = n + (n+1)^(n+2).
 * https://oeis.org/A155499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (n+1)^(n+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* convert the index to seq_t once */
    seq_t n_seq = (seq_t)n;

    /* base = n+1, exponent = n+2 */
    seq_t base = n_seq + 1;
    seq_t exp  = n_seq + 2;   /* safe addition in seq_t */

    /* compute (n+1)^(n+2) using the provided big_pow helper */
    seq_t pow_val = big_pow(base, exp);

    /* a(n) = n + pow_val */
    return n_seq + pow_val;
}
