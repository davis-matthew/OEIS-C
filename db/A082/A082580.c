/*
 * A082580  A sum of Lah numbers and binomial coefficients.
 * https://oeis.org/A082580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum[ Lah[ n, k ] binomial[ 2 k, k ], { k, 0, n } ] (with
 *            Lah[n,k] = binomial[n-1,k-1] * n! / k! for 1 ≤ k ≤ n, a(0)=1 )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is the first term */
    if (n == 0) return (seq_t)1;

    /* Compute n! once */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t result = 0;
    seq_t fact_k = 1;            /* k! (starts with 1! before first iteration) */

    for (idx_t k = 1; k <= n; ++k)
    {
        if (k > 1)
            fact_k *= (seq_t)k;  /* update to k! */

        /* Lah[n,k] = binomial[n-1,k-1] * n! / k!   (exact division) */
        seq_t bin1   = binomial(n - 1, k - 1);
        seq_t lah    = bin1 * fact_n / fact_k;

        /* Binomial[2k,k] */
        seq_t bin2   = binomial(2 * k, k);

        /* Add term to result */
        result += lah * bin2;
    }

    return result;
}
