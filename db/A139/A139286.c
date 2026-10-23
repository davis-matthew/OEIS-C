/*
 * A139286  a(n) = 2^(2*prime(n) - 1).
 * https://oeis.org/A139286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2^(2*prime(n)-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* get the nth prime (indexing starts at 1) */
    idx_t p = get_prime(n);              /* p fits in idx_t (≤ 104729 for n≤10000) */

    /* exponent = 2*p - 1, computed in seq_t to avoid overflow */
    seq_t exponent = (seq_t)((seq_t)2 * (seq_t)p - (seq_t)1);

    /* compute 2^exponent using arbitrary-precision exponentiation */
    seq_t result = pow_seq_large((seq_t)2, exponent);

    return result;
}
