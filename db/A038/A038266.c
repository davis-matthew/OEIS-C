/*
 * A038266  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*12^j.
 * https://oeis.org/A038266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 6^(i-j) * 12^j, where the non‑negative
 *            integers i, j are determined from n by `n = i*(i+1)/2 + j` (0 ≤
 *            j ≤ i). Equivalently, a(n) = binomial(i, j) * 6^i * 2^j
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

/* Compute a(n) for OEIS A038266. */
seq_t a(idx_t n)
{
    /* Locate the row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t i;
    {
        long double d = sqrtl(8.0L * n + 1.0L);
        i = (idx_t)((d - 1.0L) / 2.0L);

        /* Correct possible rounding errors. */
        while (((unsigned __int128)(i + 1) * (i + 2)) / 2 <= n) ++i;
        while (((unsigned __int128)i * (i + 1)) / 2 > n)   --i;
    }

    /* Position j within the row. */
    idx_t prior = (idx_t)(((unsigned __int128)i * (i + 1)) / 2);
    idx_t j = n - prior;

    /* binomial(i, j) */
    seq_t comb = binomial(i, j);

    /* Compute 6^i. */
    seq_t p6 = 1;
    for (idx_t k = 0; k < i; ++k)
        p6 *= (seq_t)6;

    /* Compute 2^j. */
    seq_t p2 = 1;
    for (idx_t k = 0; k < j; ++k)
        p2 *= (seq_t)2;

    /* a(n) = comb * 6^i * 2^j */
    seq_t result = comb;
    result *= p6;
    result *= p2;
    return result;
}
