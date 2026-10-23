/*
 * A183958  Number of strings of numbers x(i=1..7) in 0..n with sum i^2*x(i)
 *          equal to n*49.
 * https://oeis.org/A183958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7-tuples (x1,…,x7) with 0≤xi≤n and * 1·x1 +
 *            4·x2 + 9·x3 + 16·x4 + 25·x5 + 36·x6 + 49·x7 = 49·n
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 82, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* target sum is 49·n */
    size_t target = (size_t)(49ULL * n);
    size_t sz = target + 1;

    /* use unsigned __int128 for intermediate counts – fits up to (n+1)^7 */
    unsigned __int128 *dp1 = (unsigned __int128 *)calloc(sz, sizeof(unsigned __int128));
    unsigned __int128 *dp2 = (unsigned __int128 *)calloc(sz, sizeof(unsigned __int128));
    if (!dp1 || !dp2) {
        /* allocation failed – return 0 as safe failure value */
        free(dp1);
        free(dp2);
        return 0;
    }

    dp1[0] = 1;

    const int weights[7] = {1, 4, 9, 16, 25, 36, 49};
    for (int wi = 0; wi < 7; ++wi) {
        int w = weights[wi];
        /* dp2 is zeroed explicitly for each weight */
        memset(dp2, 0, sz * sizeof(unsigned __int128));

        for (int r = 0; r < w; ++r) {
            unsigned __int128 sum = 0;
            size_t cnt = 0;
            for (size_t i = r; i <= target; i += w, ++cnt) {
                sum += dp1[i];
                if (cnt > n) {
                    /* subtract the element that leaves the window of size n+1 */
                    sum -= dp1[i - (size_t)w * (n + 1)];
                }
                dp2[i] = sum;
            }
        }

        /* swap dp1 and dp2 for next iteration */
        unsigned __int128 *tmp = dp1;
        dp1 = dp2;
        dp2 = tmp;
    }

    seq_t result = (seq_t)dp1[target];

    free(dp1);
    free(dp2);
    return result;
}
