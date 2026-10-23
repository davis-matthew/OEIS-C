/*
 * A161287  Number of partitions of n into numbers not divisible by 3 where
 *          every part appears at least 9 times.
 * https://oeis.org/A161287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1,3∤k} (1 + x^{9k} + x^{10k} +
 *            …)
 * Range:     u8 n <= 99, u16 n <= 242, u32 n <= 685, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Products over positive integers k that are not divisible by 3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *memo = NULL;          /* pointer to array of seq_t */
    static idx_t memo_size = 0;         /* number of entries computed (0..memo_size) */

    /* trivial case: n < 1 never requested, but guard for safety */
    if (n == 0) return 0;

    /* recompute memo table up to n if necessary */
    if (memo_size < n)
    {
        idx_t N = n;
        /* allocate new dp array */
        seq_t *dp = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
        seq_t *tmp = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));

        dp[0] = 1;                      /* empty partition */

        for (idx_t a = 1; a <= N; ++a)
        {
            if (a % 3 == 0) continue;   /* skip multiples of 3 */

            /* zero temporary array */
            for (idx_t i = 0; i <= N; ++i)
                tmp[i] = 0;

            /* shift by 9 copies of part 'a' */
            idx_t shift = 9 * a;
            if (shift <= N)
            {
                for (idx_t s = shift; s <= N; ++s)
                    tmp[s] = dp[s - shift];
            }

            /* unlimited additional copies of part 'a' */
            for (idx_t s = a; s <= N; ++s)
                tmp[s] += tmp[s - a];

            /* add contributions of using part 'a'  (or not using it) */
            for (idx_t s = 0; s <= N; ++s)
                dp[s] += tmp[s];
        }

        /* free old cache and install new one */
        free(tmp);
        free(memo);
        memo = dp;
        memo_size = N;
    }

    return memo[n];
}
