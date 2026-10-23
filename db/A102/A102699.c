/*
 * A102699  Number of strings of length n, using as symbols numbers from the
 *          set {1, 2, ..., n}, in which consecutive symbols differ by exactly
 *          1.
 * https://oeis.org/A102699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*2^(n-1) - (n even? 4*(n-1)*C(n-2,(n-2)/2) :
 *            (2*n-1)*C(n-1,(n-1)/2))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of strings of length n over {1,…,n} with each
   adjacent pair differing by exactly 1. */
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp)          */
#include "../utils/binom.h"  /* provides seq_t binom(idx_t N, idx_t K) */

seq_t a(idx_t n)
{
    /* Handle the small offsets */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;

    /* (n+1) * 2^(n-1) */
    seq_t term = (seq_t)(n + 1) * pow2(n - 1);

    /* Subtract the binomial correction */
    seq_t bin;
    if ((n & 1u) == 0u)            /* even n */
    {
        bin  = binom(n - 2, (n - 2) / 2);
        term = term - bin * ((seq_t)4 * (seq_t)(n - 1));
    }
    else                           /* odd n  */
    {
        bin  = binom(n - 1, (n - 1) / 2);
        term = term - bin * (((seq_t)2 * (seq_t)n) - 1);
    }

    return term;
}
