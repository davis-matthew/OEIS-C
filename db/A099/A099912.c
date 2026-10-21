/*
 * A099912  Number of closed walks on the Herschel graph.
 * https://oeis.org/A099912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=32, a(n)=13*a(n-1)-22*a(n-2) for n>=3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation table */
    static seq_t *memo   = NULL;
    static idx_t memo_len = 0;   /* number of stored elements       */
    static idx_t memo_cap = 0;   /* allocated capacity of memo array */

    /* Initialise storage on first call */
    if (memo_len == 0) {
        memo_cap = 8;                    /* initial capacity              */
        memo     = (seq_t *)malloc(sizeof(seq_t) * memo_cap);
        if (!memo) exit(1);

        /* Base values for n = 0, 1, 2 */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)4;
        memo[2] = (seq_t)32;
        memo_len = 3;
    }

    /* Grow the table if it does not contain up to n */
    if (n >= memo_len) {
        if (n >= memo_cap) {
            /* Double the capacity until it exceeds n */
            idx_t new_cap = memo_cap;
            while (new_cap <= n) new_cap <<= 1U;
            seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * new_cap);
            if (!tmp) exit(1);
            memo     = tmp;
            memo_cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = memo_len; i <= n; ++i) {
            seq_t val = memo[i-1] * (seq_t)13u - memo[i-2] * (seq_t)22u;
            memo[i]   = val;
        }
        memo_len = n + 1;
    }

    return memo[n];
}
