/*
 * A093527  Denominators of even raw moments in the distribution of line
 *          lengths for lines picked at random in the unit disk.
 * https://oeis.org/A093527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((n+1) / binomial(2(n+1), n+1))
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_binom2N_N, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = (n+1) / gcd(n+1, binomial(2(n+1), n+1)) */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/exp_binom2N_N.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* N = n + 1 */
    idx_t N = n + 1;
    seq_t Nseq = (seq_t)N;            /* cast once for seq_t arithmetic */

    /* Compute gcd(N, binom(2N, N)) by factoring N */
    seq_t gcd_val = 1;
    idx_t temp = N;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t expN = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++expN;
            }

            idx_t expBin = exp_binom2N_N(N, p);
            idx_t minExp = expN < expBin ? expN : expBin;
            while (minExp--)
                gcd_val *= (seq_t)p;
        }
    }

    if (temp > 1)                      /* remaining prime factor */
    {
        idx_t p = temp;
        idx_t expN = 1;
        idx_t expBin = exp_binom2N_N(N, p);
        idx_t minExp = expN < expBin ? expN : expBin;
        while (minExp--)
            gcd_val *= (seq_t)p;
    }

    /* a(n) = N / gcd(N, binom(2N,N)) */
    return Nseq / gcd_val;
}
