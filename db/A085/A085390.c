/*
 * A085390  a(n) = (n(n+1)^(n-2)+0^(n-2))/(n+1).
 * https://oeis.org/A085390
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)^(n-3) for n>2; a(2)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the big_pow helper from the utils library. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 2)                     /* special case for n=2 */
        return (seq_t)1;

    /* For n ≥ 3: a(n) = n*(n+1)^(n-3) */
    seq_t base = (seq_t)(n + 1);
    seq_t exp  = (seq_t)(n - 3);
    seq_t powterm = big_pow(base, exp);

    return (seq_t)n * powterm;
}
