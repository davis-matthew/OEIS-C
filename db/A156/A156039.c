/*
 * A156039  Number of compositions (ordered partitions) of n into 4 parts,
 *          where the first is at least as great as each of the others.
 * https://oeis.org/A156039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{a=0}^{n} ( C(n-a+2,2) * - 3·C(n-2a+1,2) * +
 *            3·C(n-4a,2) * - C(n-6a-1,2) ) * where C(m,2) = m·(m−1)/2 (taken
 *            as 0 if m<2)
 * Range:     u8 n <= 15, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom2_idx.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t a = 0; a <= n; ++a)
    {
        idx_t s = n - a;               /* remaining sum for (b,c,d) */
        seq_t cnt = binom2_idx(s + 2);  /* unrestricted compositions */

        /* subtract compositions where one part exceeds a */
        if (s >= a + 1)
        {
            idx_t t = s - (a + 1);
            cnt -= 3 * binom2_idx(t + 2);
        }

        /* add back where two parts exceed a */
        if (s >= 2 * (a + 1))
        {
            idx_t t = s - 2 * (a + 1);
            cnt += 3 * binom2_idx(t + 2);
        }

        /* subtract where all three parts exceed a */
        if (s >= 3 * (a + 1))
        {
            idx_t t = s - 3 * (a + 1);
            cnt -= binom2_idx(t + 2);
        }

        res += cnt;
    }
    return res;
}
