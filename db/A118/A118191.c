/*
 * A118191  Row sums of triangle A118190: a(n) = Sum_{k=0..n} 5^(k*(n-k)) for
 *          n>=0.
 * https://oeis.org/A118191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^(k*(n-k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A118191 */
/* Uses the helper big_pow(base, exp) for efficient exponentiation */
/* Assumes seq_t and idx_t are defined elsewhere with full integer support */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;               /* accumulator */
    const seq_t base = 5;        /* base 5 for powers */
    idx_t half = n / 2;          /* compute only up to symmetry point */

    for (idx_t k = 0; k <= half; ++k)
    {
        idx_t exp = k * (n - k);            /* k*(n-k) fits in idx_t */
        seq_t term = big_pow(base, (seq_t)exp); /* 5^exp */

        if (k == n - k)                   /* middle term when n even */
        {
            sum += term;
        }
        else                              /* add pair twice */
        {
            sum += term;
            sum += term;
        }
    }

    return sum;
}
