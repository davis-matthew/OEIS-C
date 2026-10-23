/*
 * A126130  a(n) = (n+1)^n - n!.
 * https://oeis.org/A126130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)^n – n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include helper functions */
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* compute (n+1)^n */
    seq_t power = big_pow((seq_t)n + 1, (seq_t)n);

    /* compute n! */
    seq_t fact = factorial(n);

    /* return the difference */
    return power - fact;
}
