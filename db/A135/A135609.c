/*
 * A135609  a(n) = (n!)^2 - n^n.
 * https://oeis.org/A135609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 - n^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = factorial((idx_t)n);

    /* compute n^n */
    seq_t pow_n = big_pow((seq_t)n, (idx_t)n);

    /* (n!)^2 - n^n */
    seq_t result = fact * fact;
    result -= pow_n;
    return result;
}
