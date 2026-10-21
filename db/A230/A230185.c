/*
 * A230185  Number of 2 X n 0..2 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          2-x(i,j).
 * https://oeis.org/A230185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 7*a(n-2) + a(n-3) - 6*a(n-4) - 5*a(n-5) (n>6)
 *            initial values: a(1)=0, a(2)=9, a(3)=27, a(4)=159, a(5)=825,
 *            a(6)=4395
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t   *dp      = NULL;   /* dp[i] holds a(i) */
    static bool    *calc    = NULL;   /* calc[i] true if dp[i] is computed */
    static size_t   cap     = 0;       /* allocated size of the arrays       */
    static idx_t    max_n   = 0;       /* largest index already computed      */

    if (n == 0) return 0;  /* index 0 is outside the offset */

    /* enlarge the cache if necessary */
    if ((size_t)n >= cap)
    {
        size_t new_cap = (cap) ? (cap << 1) : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        dp   = (seq_t *)realloc(dp,   new_cap * sizeof(seq_t));
        calc = (bool  *)realloc(calc, new_cap * sizeof(bool));
        for (size_t i = cap; i < new_cap; ++i) calc[i] = false;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)       val = 0;
        else if (i == 2)  val = 9;
        else if (i == 3)  val = 27;
        else if (i == 4)  val = 159;
        else if (i == 5)  val = 825;
        else if (i == 6)  val = 4395;
        else
        {
            val = 4*dp[i-1] + 7*dp[i-2] + dp[i-3] - 6*dp[i-4] - 5*dp[i-5];
        }
        dp[i]   = val;
        calc[i] = true;
    }
    if (n > max_n) max_n = n;

    return dp[n];
}
