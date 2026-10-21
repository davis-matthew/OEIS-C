/*
 * A092301  a(n) = 3^(n^2+2n+1)*Product_{j=1..n} (9^j-1).
 * https://oeis.org/A092301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n^2+2n+1) * Product_{j=1..n} (9^j – 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* for big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)3;

    /* Compute prod = ∏_{j=1}^n (9^j – 1) */
    seq_t prod = 1;      /* holding the running product */
    seq_t pow9 = 1;      /* current value of 9^j */

    for (idx_t j = 1; j <= n; ++j)
    {
        pow9 = pow9 * (seq_t)9;          /* pow9 = 9^j */
        seq_t term = pow9 - (seq_t)1;    /* (9^j – 1) */
        prod *= term;                    /* multiply into the product */
    }

    /* Compute 3^( (n+1)^2 ) */
    seq_t exp = (seq_t)(n + 1) * (seq_t)(n + 1);  /* (n+1)^2 */
    seq_t pow3 = big_pow((seq_t)3, exp);

    /* Final result */
    return prod * pow3;
}
