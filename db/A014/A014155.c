/*
 * A014155  Sum of a nonnegative cube and a triangular number.
 * https://oeis.org/A014155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonnegative integer that can be expressed as m³ +
 *            k(k+1)/2 for some integers m≥0, k≥0
 * Range:     u8 n <= 51, u16 n <= 9749, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is defined for n≥1, but return 0 for safety */

    idx_t bound = (idx_t)(4 * n) + 10;   /* heuristic upper bound */
    if (bound < 10) bound = 10;

    while (1)
    {
        /* allocate a bit array to record which numbers up to bound are representable */
        unsigned char *present = (unsigned char *)calloc((size_t)(bound + 1), 1);
        if (!present) return 0;    /* allocation failed – return 0 as a safe fallback */

        /* generate all sums m³ + T_k that do not exceed bound */
        for (idx_t m = 0; ; ++m)
        {
            seq_t cube = (seq_t)m * m * m;
            if (cube > (seq_t)bound) break;

            for (idx_t k = 0; ; ++k)
            {
                seq_t t    = (seq_t)k * (k + 1) / 2;
                seq_t sum  = cube + t;
                if (sum > (seq_t)bound) break;
                present[(size_t)sum] = 1;
            }
        }

        /* scan the array to find the n-th representable number */
        idx_t count = 0;
        for (idx_t val = 0; val <= bound; ++val)
        {
            if (present[(size_t)val])
            {
                ++count;
                if (count == n)
                {
                    free(present);
                    return (seq_t)val;
                }
            }
        }

        /* not enough terms found – increase the bound and retry */
        free(present);
        bound *= 2;
    }
}
