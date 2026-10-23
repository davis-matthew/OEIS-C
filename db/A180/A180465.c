/*
 * A180465  Squares which are a sum of consecutive squares starting with 38^2.
 * https://oeis.org/A180465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = the nth square m^2 such that m^2 = Σ_{k=38}^{m} k^2, for
 *            m ≥ 38. The sequence lists the square values
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided sqrt routine for arbitrary precision */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t CONST = (seq_t)17575;   /* Σ_{k=1}^{37} k^2 */
    idx_t count = 0;

    for (idx_t m = 38; ; ++m)
    {
        seq_t mt   = (seq_t)m;
        seq_t sum  = mt * (mt + 1) * (2 * mt + 1) / 6;
        seq_t diff = sum - CONST;              /* the candidate square */

        seq_t r = floor_sqrt(diff);
        if (r * r == diff)                    /* perfect square */
        {
            ++count;
            if (count == n)
                return diff;
        }
    }
}
