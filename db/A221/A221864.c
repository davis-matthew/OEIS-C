/*
 * A221864  Number of forests (sets) of rooted labeled trees on {1,2,...,n}
 *          such that the node with label 1 is in the same rooted tree as the
 *          node with label 2.
 * https://oeis.org/A221864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-2}
 *            binomial(n-2,k)*(k+2)^(k+1)*(n-k-1)^(n-k-3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* a(0)=0, a(1)=0 */

    seq_t sum = 0;
    idx_t n_minus_2 = n - 2;

    for (idx_t k = 0; k <= n_minus_2; ++k)
    {
        seq_t bin  = binomial(n_minus_2, k);
        seq_t p1   = seq_pow((seq_t)(k + 2), k + 1);

        idx_t base2 = n - k - 1;          /* base for the second power */
        seq_t p2;
        if (base2 == 1)                    /* handles the k = n-2 case */
            p2 = 1;
        else
        {
            idx_t exp2 = n - k - 3;        /* exponent is ≥ 0 when base2 > 1 */
            p2 = seq_pow((seq_t)base2, exp2);
        }

        sum += bin * p1 * p2;
    }

    return sum;
}
