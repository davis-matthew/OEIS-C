/*
 * A229036  G.f.: Sum_{n>=0} (3*n-1)^n * x^n / (1 + (3*n-1)*x)^n.
 * https://oeis.org/A229036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)*3^(n-1)*n!/2 for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* (3*n + 1) */
    seq_t part = ((seq_t)3 * (seq_t)n) + 1;

    /* 3^(n-1) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));

    /* n! */
    seq_t fact = factorial(n);

    /* Cancel the division by 2 early to avoid overflow. */
    if (n & 1)           /* n odd: (3*n+1) is even */
        part /= 2;
    else                 /* n even: n! is even for n>=2 */
        fact /= 2;

    /* Multiply the parts: part * pow3 * fact */
    seq_t result = part * pow3;
    result *= fact;

    return result;
}
