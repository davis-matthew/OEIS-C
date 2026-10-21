/*
 * A084439  Number of triangular partitions of n of order 3.
 * https://oeis.org/A084439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i≥0, j≥0, k≥0, i + 3*j + 5*k = n} C(i+2,2) * (j+1)
 *            where C(m,2) = m*(m-1)/2. This is derived from the generating
 *            function 1/((1-x)^3 * (1-x^3)^2 * (1-x^5))
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 464, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the coefficient of x^n in that series. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t j = 0; 3 * j <= n; ++j)       /* number of 3‑parts (two colours) */
    {
        idx_t rem_after_3 = n - 3 * j;

        for (idx_t k = 0; 5 * k <= rem_after_3; ++k)   /* number of 5‑parts (one colour) */
        {
            idx_t i = rem_after_3 - 5 * k;             /* remaining ones (three colours) */

            /* Compute C(i+2,2) = (i+2)*(i+1)/2 using a wider type to avoid intermediate overflow */
            unsigned __int128 comb = (unsigned __int128)(i + 2) * (i + 1) / 2;

            unsigned __int128 term = comb * ((unsigned __int128)j + 1);

            result += (seq_t)term;
        }
    }

    return result;
}
