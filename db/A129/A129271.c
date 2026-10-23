/*
 * A129271  Number of labeled n-node connected graphs with at most one cycle.
 * https://oeis.org/A129271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n-2) + (n-1)(n-2)/2 * sum_{t=0}^{n-3} ( (n-3)! / t! )
 *            * n^t
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(0)=1, a(1)=1, a(2)=1. The sum is empty for n<3. */

#include "../utils/big_pow.h"   /* for big_pow(seq_t, idx_t)                 */
#include "../utils/factorial.h" /* for factorial(idx_t)                      */

seq_t a(idx_t n)
{
    /* Base cases  */
    if (n == 0 || n == 1)
        return (seq_t)1;          /* a(0)=a(1)=1                                */
    if (n == 2)
        return (seq_t)1;          /* a(2)=1                                       */

    /* Trees:  n^(n-2)  */
    seq_t trees = big_pow((seq_t)n, n - 2);

    /* Factorial part (n-3)!  */
    seq_t fact = factorial(n - 3);

    /* Sum_{t=0}^{n-3} ( (n-3)! / t! ) * n^t  */
    seq_t sum   = 0;
    seq_t term  = fact;                 /* term for t=0  = (n-3)!                   */
    for (idx_t t = 0; t <= n - 3; ++t) {
        sum += term;
        if (t < n - 3) {                /* update for next t */
            term = (term * (seq_t)n) / (t + 1);
        }
    }

    /* Multiplier (n-1)(n-2)/2  */
    seq_t mult = ( (seq_t)(n - 1) * (seq_t)(n - 2) ) / 2;

    /* Final result  */
    return trees + mult * sum;
}
