/*
 * A118421  Number of integer solutions to n = x^2 + (2y)^2 + z(z+1)/2.
 * https://oeis.org/A118421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to n = x^2 + (2y)^2 +
 *            z(z+1)/2
 * Range:     u8 n <= 63, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <unistd.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t total = 0;

    /* Find maximal m such that T_m = m(m+1)/2 <= n */
    idx_t m = 0;
    while (((seq_t)m * ((seq_t)m + 1)) / 2 <= (seq_t)n) m++;
    idx_t m_max = m - 1;

    for (idx_t i = 0; i <= m_max; ++i)
    {
        /* Triangular number and remaining part */
        seq_t tri = (seq_t)i * ((seq_t)i + 1) / 2;
        seq_t rem = (seq_t)n - tri;

        /* Count solutions of x^2 + 4y^2 = rem */
        seq_t cnt = 0;
        seq_t is = floor_sqrt(rem);
        int64_t xlim = (int64_t)is;

        for (int64_t x = -xlim; x <= xlim; ++x)
        {
            seq_t xsq = (seq_t)x * x;
            seq_t rem2 = rem - xsq;
            if (rem2 % 4 != 0) continue;

            seq_t k = rem2 / 4;
            seq_t msq = floor_sqrt(k);
            if (msq * msq == k)
            {
                if (msq == 0) cnt += 1;      /* y = 0 only one sign */
                else          cnt += 2;      /* y = ±m */
            }
        }

        /* For each triangular number there are two distinct z (i and -i-1) */
        total += cnt * 2;
    }

    return total;
}
