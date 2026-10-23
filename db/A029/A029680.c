/*
 * A029680  Theta series of quadratic form with Gram matrix [ 4, 2, 2; 2, 4,
 *          1; 2, 1, 6 ].
 * https://oeis.org/A029680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (i,j,k) to 4i^2 + 4j^2 + 4k^2
 *            + 2ij + 2ik + 2jk = 2n . This is equivalent to counting integer
 *            solutions of i^2 + 3j^2 + 5k^2 = 2n with the appropriate
 *            multiplicities
 * Range:     u8 n <= 593, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple integer square root: floor(sqrt(v)) for 0 <= v <= 2^63-1. */
static inline idx_t isqrt(idx_t v)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v) ++r;
    return r;
}

/* Compute a(n) for the OEIS sequence A029680. */
seq_t a(idx_t n)
{
    /* Two times the target value: 2*n.  Use idx_t to avoid overflow regardless
       of the size of seq_t. */
    idx_t two_n = 2 * n;
    idx_t total = 0;  /* accumulator for the count */

    /* Loop over j: 3*j^2 <= two_n  ->  j <= floor(sqrt(two_n/3)). */
    idx_t jmax = isqrt(two_n / 3);

    for (idx_t j = 0; j <= jmax; ++j)
    {
        idx_t j_sq   = j * j;
        idx_t tj     = 3 * j_sq;
        idx_t j_mult = (j == 0 ? 1 : 2);  /* multiplicity for +/- j */

        if (tj > two_n) continue;        /* safety check (should not happen) */

        /* Loop over k: 5*k^2 <= two_n - tj  ->  k <= floor(sqrt((two_n - tj)/5)). */
        idx_t kmx = isqrt((two_n - tj) / 5);

        for (idx_t k = 0; k <= kmx; ++k)
        {
            idx_t k_sq   = k * k;
            idx_t dk     = 5 * k_sq;
            idx_t rem    = two_n - tj - dk;

            /* Check whether rem is a perfect square. */
            idx_t s = isqrt(rem);
            if (s * s != rem) continue;   /* remainder is not a square */

            idx_t f = (rem == 0 ? 1 : 2); /* multiplicity for +/- i part */
            idx_t k_mult = (k == 0 ? 1 : 2);

            total += j_mult * k_mult * f;
        }
    }

    return (seq_t)total;
}
