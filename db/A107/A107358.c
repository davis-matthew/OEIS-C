/*
 * A107358  Dying rabbits: a(n) = Fibonacci(n) for n <= 12; for n >= 13, a(n)
 *          = a(n-1) + a(n-2) - a(n-13).
 * https://oeis.org/A107358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=0, a(1)=1; 
   a(n)=a(n-1)+a(n-2)            for 2 <= n <= 12;
   a(n)=a(n-1)+a(n-2)-a(n-13)    for n >= 13
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* dynamic array of terms */
    static idx_t sz  = 0;      /* current allocated size         */
    static idx_t last = 0;     /* highest index computed so far   */
    static int   init = 0;     /* flag whether dp[0],dp[1] set   */

    /* Grow the array if necessary */
    if (n >= sz)
    {
        idx_t new_sz = (sz == 0) ? 64 : sz * 2;
        while (new_sz <= n) new_sz <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        dp = tmp;
        sz = new_sz;
    }

    /* Initialise the first two terms on first use */
    if (!init)
    {
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        last   = 1;
        init   = 1;
    }

    /* Return a cached value if we already computed it */
    if (n <= last) return dp[n];

    /* Compute the sequence up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i <= 12)
            dp[i] = dp[i - 1] + dp[i - 2];
        else
            dp[i] = dp[i - 1] + dp[i - 2] - dp[i - 13];
    }
    last = n;
    return dp[n];
}
