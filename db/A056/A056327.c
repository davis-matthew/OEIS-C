/*
 * A056327  Number of reversible string structures with n beads using exactly
 *          three different colors.
 * https://oeis.org/A056327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 24*a(n-3) + 49*a(n-4) + 6*a(n-5) -
 *            66*a(n-6) + 36*a(n-7) for n ≥ 8 with initial values a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=4, a(5)=15, a(6)=50, a(7)=160
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based memoisation array (memo[1] = a(1)) */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;   /* current highest index cached */

    if (n == 0) return 0;        /* safety for accidental 0 */

    /* extend memo array if necessary */
    if (n > memo_len)
    {
        idx_t new_len = n + 1;   /* allocate indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(memo, (size_t)new_len * sizeof(seq_t));
        if (tmp == NULL) abort(); /* allocation failed */
        memo = tmp;
        memo_len = n;

        /* initialise new entries */
        for (idx_t i = 1; i <= n; ++i)
        {
            if (i <= 7)   /* default initial values */
            {
                switch (i)
                {
                    case 1: memo[i] = 0; break;
                    case 2: memo[i] = 0; break;
                    case 3: memo[i] = 1; break;
                    case 4: memo[i] = 4; break;
                    case 5: memo[i] = 15; break;
                    case 6: memo[i] = 50; break;
                    case 7: memo[i] = 160; break;
                }
            }
            else if (i <= 7) { /* already set just in case */ }
            else            /* compute using recurrence */
            {
                seq_t pos = 6ULL * memo[i-1]
                           + 49ULL * memo[i-4]
                           + 6ULL * memo[i-5]
                           + 36ULL * memo[i-7];

                seq_t neg = 6ULL * memo[i-2]
                           + 24ULL * memo[i-3]
                           + 66ULL * memo[i-6];

                memo[i] = pos - neg;   /* guaranteed non-negative in theory */
            }
        }
    }

    return memo[n];
}
