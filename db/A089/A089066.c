/*
 * A089066  Number of distinct classes of permutations of length n under
 *          reversal, rotation and complement to n+1.
 * https://oeis.org/A089066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! + (n/2+1)(n/2)!2^(n/2)) / (4n) for even n ≥ 4 a(n) =
 *            ((n-1)! + ((n-1)/2)!2^((n-1)/2)) / 4 for odd n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                  a(1)=1  a(2)=1 are handled explicitly               */

#include "../utils/factorial.h"   /* seq_t factorial(idx_t n); */
#include "../utils/pow2.h"        /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 2)
        return (seq_t)1;

    if ((n & 1) == 0)                  /* n even */
    {
        idx_t k      = n / 2;
        seq_t fact_n = factorial(n);
        seq_t fact_k = factorial(k);
        seq_t pow_k  = pow2(k);        /* 2^k */

        seq_t term   = fact_k * pow_k; /* (k)! * 2^k  */
        seq_t mult   = (seq_t)(k + 1) * term; /* (k+1) * term */
        seq_t sum    = fact_n + mult;
        seq_t denom  = (seq_t)(4 * n);

        return sum / denom;
    }
    else                               /* n odd */
    {
        idx_t k         = (n - 1) / 2;
        seq_t fact_n1   = factorial(n - 1);
        seq_t fact_k    = factorial(k);
        seq_t pow_k     = pow2(k);      /* 2^k */

        seq_t term   = fact_k * pow_k;  /* k! * 2^k  */
        seq_t sum    = fact_n1 + term;
        seq_t denom  = (seq_t)4;

        return sum / denom;
    }
}
