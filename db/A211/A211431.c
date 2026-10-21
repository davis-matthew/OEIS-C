/*
 * A211431  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^3+(x+y)^2=0.
 * https://oeis.org/A211431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(w,x,y)∈[-n,n]^3 : w^3 + (x+y)^2 = 0}|
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; ; ++k)
    {
        /* t = k^2; check t <= n */
        unsigned __int128 t = (unsigned __int128)k * k;
        if (t > n) break;

        if (k == 0)
        {
            /* w = 0, s = 0; pairs count = 2n + 1 */
            unsigned __int128 cnt0 = (unsigned __int128)2 * n + 1;
            total += (seq_t)cnt0;
            continue;
        }

        /* s = k^3 = t * k */
        unsigned __int128 s128 = t * k;
        /* if |s| > 2n, no solutions */
        if (s128 > (unsigned __int128)2 * n) continue;

        idx_t s = (idx_t)s128;          /* s >= 0 */
        unsigned __int128 pairs;
        if (s <= n)
        {
            /* count for |s| <= n: 2n + 1 - s */
            pairs = ((unsigned __int128)2 * n + 1) - s;
        }
        else
        {
            /* count for n < |s| <= 2n: 2n - s + 1 */
            pairs = ((unsigned __int128)2 * n) - s + 1;
        }
        /* both signs of s are possible */
        total += (seq_t)(pairs * 2);
    }
    return total;
}
