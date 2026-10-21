/*
 * A064047  Number of numbers only appearing once in 1-to-n multiplication
 *          table.
 * https://oeis.org/A064047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers x in [1,n] such that x^2 has no
 *            divisor d with x < d <= n (i.e., x^2 appears exactly once in the
 *            1‑to‑n multiplication table)
 * Range:     u8 n <= 473, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t x = 1; x <= n; ++x)
    {
        if (x == 1) { result++; continue; }

        /* Compute x^2 using 128-bit to avoid overflow, then find the smallest
           divisor i >= ceil(x^2 / n).  If that start point is already >= x,
           then no divisor in the interval [ceil(x^2/n), x-1] exists,
           so x^2 appears only once. */
        unsigned __int128 prod   = (unsigned __int128)x * (unsigned __int128)x;
        unsigned __int128 n128   = (unsigned __int128)n;
        unsigned __int128 start128 = (prod + n128 - 1) / n128;   /* ceil(prod / n) */
        idx_t start = (idx_t)start128;

        if (start >= x) { result++; continue; }

        bool unique = true;
        for (idx_t i = start; i < x; ++i)
        {
            if (prod % (unsigned __int128)i == 0)
            {
                unique = false;
                break;
            }
        }
        if (unique) result++;
    }
    return result;
}
