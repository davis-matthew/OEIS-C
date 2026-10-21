/*
 * A217628  a(n) = 3^((n-1)*(n+2)/2).
 * https://oeis.org/A217628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^((n-1)*(n+2)/2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 */
    if (n == 0)
        return 0;

    /* Compute the exponent as (n-1)*(n+2)/2, avoiding overflow on idx_t
       by performing the multiplication in seq_t first. */
    seq_t exp_seq = ((seq_t)(n - 1)) * ((seq_t)(n + 2));
    exp_seq /= 2;

    /* The exponent now fits in idx_t for all practical values of n. */
    idx_t exp = (idx_t)exp_seq;

    /* Return 3^exp using the provided helper. */
    return pow3(exp);
}
