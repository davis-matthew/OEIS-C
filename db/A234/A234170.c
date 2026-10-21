/*
 * A234170  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by recurrence a(1) = 448, a(2) = 4536, a(3) =
 *            47936, a(4) = 517104, a(5) = 5630848, a(6) = 61624416, a(n) =
 *            154*a(n-2) - 4144*a(n-4) + 16176*a(n-6) for n >= 7 (OEIS
 *            A234170). The sequence is memoized for fast reuse
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10010];
static idx_t cached_len = 0;   /* highest index currently stored in cache */

/* Return the number of (n+1)×2 arrays with the diagonal–antidiagonal
   sum difference equal to 1 for every 2×2 subblock.  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  */
    if (n == 0) return 0;   /* Undefined for n=0; return 0 for safety. */

    /* Expand cache up to requested n.  */
    if (cached_len < n)
    {
        /* Initialize base values if not already present.  */
        if (cached_len < 6)
        {
            cache[1] = (seq_t)448;
            cache[2] = (seq_t)4536;
            cache[3] = (seq_t)47936;
            cache[4] = (seq_t)517104;
            cache[5] = (seq_t)5630848;
            cache[6] = (seq_t)61624416;
            cached_len = 6;
        }

        /* Compute remaining terms up to n using the recurrence.  */
        for (idx_t i = cached_len + 1; i <= n; ++i)
        {
            if (i <= 6)  /* safety check – values already set above   */
                continue;

            /* a(i) = 154*a(i-2) - 4144*a(i-4) + 16176*a(i-6)   */
            seq_t val = (seq_t)154 * cache[i-2];
            val += (seq_t)16176 * cache[i-6];
            val -= (seq_t)4144 * cache[i-4];
            cache[i] = val;
        }

        cached_len = n;
    }

    return cache[n];
}
