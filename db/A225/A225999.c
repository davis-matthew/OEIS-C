/*
 * A225999  Number of pairs of orthogonal (-1,7) vectors of length 8*n.
 * https://oeis.org/A225999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{k=0}^{n} C(8n, n+7k) * C(n+7k, k) * 2^{n+6k}
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (C denotes the binomial coefficient.)  */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1, a(1) is the first listed term */

    seq_t sum = 0;
    idx_t L = (idx_t)8 * n;               /* total length = 8n */
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t r1 = n + (idx_t)7 * k;      /* upper index for first binomial */
        seq_t term1 = binom(L, r1);       /* C(8n, n+7k) */
        seq_t term2 = binom(r1, k);       /* C(n+7k, k) */
        idx_t exp  = n + (idx_t)6 * k;    /* exponent for 2^(n+6k) */
        seq_t term3 = pow2(exp);          /* 2^(n+6k) */

        seq_t prod = term1 * term2 * term3;
        sum += prod;
    }

    /* unordered pairs: divide by 2 */
    sum /= 2;
    return sum;
}
