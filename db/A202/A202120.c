/*
 * A202120  Number of -4..4 arrays of n elements with first, second and third
 *          differences also in -4..4.
 * https://oeis.org/A202120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n arrays with terms and all first,
 *            second and third differences in the range [-4,4]. Computation
 *            uses dynamic programming over 3‑tuples (a_{i-2},a_{i-1},a_i).
 *            For n=1,2 counts are obtained by direct enumeration. For n>=3
 *            the recurrence is a(n) = sum_{x,y,z,w} a(n-1,x,y,z)...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   idx3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Allow unused variable warning for static arrays */
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#include <string.h>      /* for memset */
#include "../utils/idx3.h"

seq_t a(idx_t n)
{
    /* base cases for n = 1 and n = 2 */
    if (n == 0) return 0;
    if (n == 1) return 9;

    if (n == 2)
    {
        seq_t total = 0;
        for (int a = -4; a <= 4; ++a)
        {
            for (int b = -4; b <= 4; ++b)
            {
                int d = b - a;
                if (d >= -4 && d <= 4)
                    ++total;
            }
        }
        return total;
    }

    /* DP arrays for tuples of length 3: 9^3 = 729 states */
    static seq_t dp_prev[729];
    static seq_t dp_next[729];
    /* initialise dp_prev with all valid triples for n = 3 */
    memset(dp_prev, 0, sizeof(dp_prev));
    for (int x = -4; x <= 4; ++x)
    {
        for (int y = -4; y <= 4; ++y)
        {
            for (int z = -4; z <= 4; ++z)
            {
                int d1_1 = y - x;
                int d1_2 = z - y;
                int d2_1 = z - 2 * y + x;
                if (d1_1 >= -4 && d1_1 <= 4 &&
                    d1_2 >= -4 && d1_2 <= 4 &&
                    d2_1 >= -4 && d2_1 <= 4)
                {
                    int id = idx3(x, y, z);
                    dp_prev[id] = 1;
                }
            }
        }
    }

    /* propagate DP from length 3 up to length n */
    size_t steps = (size_t)(n - 3);
    for (size_t step = 0; step < steps; ++step)
    {
        memset(dp_next, 0, sizeof(dp_next));
        for (int x = -4; x <= 4; ++x)
        {
            for (int y = -4; y <= 4; ++y)
            {
                for (int z = -4; z <= 4; ++z)
                {
                    int id = idx3(x, y, z);
                    seq_t cnt = dp_prev[id];
                    if (!cnt) continue;
                    for (int w = -4; w <= 4; ++w)
                    {
                        int d1 = w - z;
                        if (d1 < -4 || d1 > 4) continue;
                        int d2 = w - 2 * z + y;
                        if (d2 < -4 || d2 > 4) continue;
                        int d3 = w - 3 * z + 3 * y - x;
                        if (d3 < -4 || d3 > 4) continue;
                        int nid = idx3(y, z, w);
                        dp_next[nid] += cnt;
                    }
                }
            }
        }
        /* move dp_next into dp_prev for next iteration */
        memcpy(dp_prev, dp_next, sizeof(dp_prev));
    }

    /* sum all counts for sequences of length n */
    seq_t total = 0;
    for (int i = 0; i < 729; ++i)
        total += dp_prev[i];

    return total;
}
#pragma GCC diagnostic pop
