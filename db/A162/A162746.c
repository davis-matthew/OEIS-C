/*
 * A162746  Row sums of Fibonacci-Pascal triangle A162745.
 * https://oeis.org/A162746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n,2k) * 2^(n-2k) * F(k+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F denotes the Fibonacci numbers (F(0)=0,F(1)=1).          */

#include <stdint.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n)            */
#include "../utils/pow2.h"  /* provides seq_t pow2(idx_t e)          */

seq_t a(idx_t n)
{
    /* Number of terms in the sum. */
    idx_t maxk = n / 2;

    seq_t sum   = 0;      /* running sum */
    seq_t binom = 1;      /* C(n,0) = 1, used for k=0 */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* 2^(n-2k) */
        seq_t pow2_term = pow2(n - 2 * k);

        /* F(k+1) */
        seq_t fib_term = fib(k + 1);

        /* Add term: binom * pow2 * fib  */
        sum += binom * pow2_term * fib_term;

        /* Compute next binomial coefficient C(n,2(k+1)) if needed */
        if (k < maxk)
        {
            /* binom_{k+1} = binom_k * (n-2k) / (2k+1)
                                 * (n-2k-1) / (2k+2) */
            seq_t t = binom * (seq_t)(n - 2 * k);
            t = t / (seq_t)(2 * k + 1);
            t = t * (seq_t)(n - 2 * k - 1);
            binom = t / (seq_t)(2 * k + 2);
        }
    }

    return sum;
}
