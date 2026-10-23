/*
 * A197539  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,2,1,1,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 68, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A197539
   Recurrence: a(1)=1, a(2)=5, a(3)=8, a(4)=19, a(5)=28, a(6)=58,
               a(7)=119, a(8)=219
   a(n) = a(n-1) + 2*a(n-3) + 4*a(n-4) - 3*a(n-5) - a(n-6)
          + 3*a(n-7) - 2*a(n-8)   (n>=9)   */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static memoisation table indexed by 1‑based n.
       Index 0 is unused.  */
    static seq_t *memo      = NULL;
    static size_t memo_cap  = 0;   /* number of slots allocated   */
    static size_t memo_len  = 0;   /* largest n computed          */
    static bool   init       = false;

    if (!init)
    {
        /* initialise up to n = 8 */
        memo_cap = 9;                         /* indices 0..8 */
        memo     = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
        if (!memo) abort();

        memo[0] = (seq_t)0;
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)5;
        memo[3] = (seq_t)8;
        memo[4] = (seq_t)19;
        memo[5] = (seq_t)28;
        memo[6] = (seq_t)58;
        memo[7] = (seq_t)119;
        memo[8] = (seq_t)219;
        memo_len = 8;
        init = true;
    }

    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    size_t nn = (size_t)n;

    /* grow table if necessary */
    if (nn >= memo_cap)
    {
        size_t new_cap = memo_cap;
        while (nn >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        memo_cap = new_cap;
    }

    /* value already computed */
    if (nn <= memo_len) return memo[nn];

    /* compute missing terms iteratively up to nn */
    for (size_t i = memo_len + 1; i <= nn; ++i)
    {
        seq_t val = memo[i - 1];
        val += (seq_t)2 * memo[i - 3];
        val += (seq_t)4 * memo[i - 4];
        val -= (seq_t)3 * memo[i - 5];
        val -= memo[i - 6];
        val += (seq_t)3 * memo[i - 7];
        val -= (seq_t)2 * memo[i - 8];
        memo[i] = val;
    }
    memo_len = nn;
    return memo[nn];
}
