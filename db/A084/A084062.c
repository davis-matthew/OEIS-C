/*
 * A084062  Main diagonal of number array A084061.
 * https://oeis.org/A084062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ((n + sqrt(n))^n + (n - sqrt(n))^n)/2
 *
 * Computation: use the integer identity
 *
 *   a(n) = Σ_{j=0}^{⌊n/2⌋} C(n,2j) · n^{n-j}
 *
 *   where C(n,k) is the binomial coefficient.
 *
 * The sum is computed iteratively:
 *   - n^n is computed once with the supplied big_pow routine.
 *   - For each step j the power is divided by n.
 *   - The binomial coefficient is updated using the recurrence
 *       C(n,2j) = C(n,2j-2) · (n-2j+2)(n-2j+1) / ((2j-1)(2j))
 *   - Terms are accumulated into the result.
 *
 * This method requires only integer arithmetic; it works for any
 * seq_t that supports +, -, *, /, comparison, etc.
 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    const seq_t sn = (seq_t)n;
    seq_t sum = (seq_t)0;

    seq_t pow = big_pow(sn, sn);          /* n^n */
    seq_t binom = (seq_t)1;               /* C(n,0) */

    idx_t maxj = n / (idx_t)2;            /* floor(n/2) */

    for (idx_t j = 0; j <= maxj; ++j) {
        /* term = binom * pow */
        seq_t term = binom * pow;
        sum += term;

        if (j == maxj)
            break;                       /* last iteration done */

        /* update power: n^(n-(j+1)) = n^(n-j) / n */
        pow = pow / sn;

        /* update binomial coefficient to C(n,2(j+1)) */
        seq_t a = (seq_t)(n - (2 * j));
        seq_t b = (seq_t)(n - (2 * j) - 1);
        seq_t numerator = binom * a;
        numerator = numerator * b;

        seq_t denom = (seq_t)((2 * j + 1) * (2 * j + 2));
        binom = numerator / denom;
    }

    return sum;
}
