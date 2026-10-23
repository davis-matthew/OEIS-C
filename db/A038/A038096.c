/*
 * A038096  Number of rooted graphs on n labeled nodes where the root has
 *          degree 3.
 * https://oeis.org/A038096
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * binomial(n-1,3) * 2^binomial(n-1,2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 4.  Return 0 for smaller n. */
    if (n < 4) return 0;

    /* Compute each factor in turn */
    seq_t res = (seq_t)n;                 /* n */
    seq_t c3  = binomial(n - 1, 3);       /* C(n-1,3) */
    seq_t c2  = binomial(n - 1, 2);       /* C(n-1,2) */

    /* Multiply by the first binomial */
    res *= c3;

    /* Compute 2^c2 using arbitrary‑precision exponentiation */
    seq_t pow2c2 = big_pow((seq_t)2, c2);

    /* Final result */
    res *= pow2c2;
    return res;
}
