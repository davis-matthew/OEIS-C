/*
 * A206514  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than four equal edges, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A206514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for OEIS A206514 Empirical recurrence for n > 7: a(n) =
 *            18*a(n-1) - 27*a(n-2) - 76*a(n-3) - 111*a(n-4) - 66*a(n-5) -
 *            24*a(n-6) Initial values: a(1)=15, a(2)=186, a(3)=2786,
 *            a(4)=43804, a(5)=697369, a(6)=11136544, a(7)=177977851
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the surrounding build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at 1 */

    /* memoization array: memo[0] is unused; memo[i] holds a(i) for i>=1 */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;        /* largest n stored */

    /* First call: allocate and store the first 7 terms */
    if (memo_size == 0) {
        memo_size = 7;
        memo = (seq_t *)malloc(sizeof(seq_t) * (memo_size + 1));
        if (!memo) return 0;           /* allocation failure */
        memo[0] = 0;                   /* unused */
        memo[1] = 15;
        memo[2] = 186;
        memo[3] = 2786;
        memo[4] = 43804;
        memo[5] = 697369;
        memo[6] = 11136544;
        memo[7] = 177977851;
    }

    /* Extend the memo array if a larger n is requested */
    if (n > memo_size) {
        size_t new_size = (size_t)(n + 1);   /* +1 so index n is valid */
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        memo = tmp;
        /* Compute new values using the recurrence */
        for (idx_t i = memo_size + 1; i <= n; ++i) {
            seq_t v1 = memo[i - 1];
            seq_t v2 = memo[i - 2];
            seq_t v3 = memo[i - 3];
            seq_t v4 = memo[i - 4];
            seq_t v5 = memo[i - 5];
            seq_t v6 = memo[i - 6];
            memo[i] = 18 * v1 - 27 * v2 - 76 * v3
                      - 111 * v4 - 66 * v5 - 24 * v6;
        }
        memo_size = n;
    }

    return memo[n];
}
