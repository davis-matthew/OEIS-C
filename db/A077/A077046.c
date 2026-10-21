/*
 * A077046  Doubly restricted composition numbers: number of compositions of
 *          [n^2/2] into exactly n positive integers each strictly less than
 *          n.
 * https://oeis.org/A077046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A077042(n-1, n) Number of compositions of floor(n^2/2)
 *            into exactly n positive parts, each strictly less than n. Uses
 *            inclusion–exclusion
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 25,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/seq_mul.h"

/* Compute binomial coefficient C(N,K) for seq_t.
   For 64‑bit seq_t use 128‑bit arithmetic to avoid overflow.
   For larger seq_t use the supplied seq_mul routine. */
static seq_t binom(idx_t N, idx_t K)
{
    if (K > N)
        return 0;
    if (K > N - K)
        K = N - K;

    if (sizeof(seq_t) <= 8)                 /* 64‑bit or smaller */
    {
        unsigned __int128 res = 1;
        for (idx_t i = 1; i <= K; ++i)
        {
            res *= (unsigned __int128)(N - K + i);
            res /= (unsigned __int128)i;    /* exact division */
        }
        return (seq_t)res;
    }
    else                                    /* seq_t is larger or big‑int */
    {
        seq_t res = 1;
        for (idx_t i = 1; i <= K; ++i)
        {
            res = seq_mul(res, (seq_t)(N - K + i));
            res /= (seq_t)i;                /* exact division */
        }
        return res;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                         /* a(1) = 0 */

    /* S = floor(n^2/2) */
    seq_t S = ((seq_t)n * (seq_t)n) / 2;

    /* Parameters for inclusion–exclusion */
    idx_t M = (idx_t)S - 1;                  /* S-1 */
    idx_t d = n - 1;                         /* upper bound + 1 */
    idx_t T = (idx_t)S - n;                  /* S - n */
    idx_t kmax = T / d;                      /* max k with non‑negative argument */

    seq_t sum = 0;                           /* cumulative result */

    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t c1 = binom(n, k);              /* C(n,k) */
        idx_t R = M - k * d;                 /* top of second binomial */
        seq_t c2;
        if (R < d)
            c2 = 0;
        else
            c2 = binom(R, d);                /* C(R, n-1) */

        seq_t term = seq_mul(c1, c2);        /* C(n,k)*C(R,n-1) */

        if ((k & 1) == 0)
            sum += term;                     /* + for even k */
        else
            sum -= term;                     /* - for odd k  */
    }

    return sum;
}
