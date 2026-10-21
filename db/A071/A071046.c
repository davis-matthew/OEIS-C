/*
 * A071046  Number of 0's in n-th row of triangle in A071031, cellular
 *          automaton "rule 62".
 * https://oeis.org/A071046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) - a(n-7) Initial values: a(0)=0 a(1)=0
 *            a(2)=2 a(3)=1 a(4)=4 a(5)=3 a(6)=4
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    if (memo_size == 0) {                     /* first call: initialise */
        memo_size = 7;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        if (!memo) return 0;                  /* out‑of‑memory – unlikely */

        memo[0] = 0;
        memo[1] = 0;
        memo[2] = 2;
        memo[3] = 1;
        memo[4] = 4;
        memo[5] = 3;
        memo[6] = 4;
    }

    if (n < memo_size)                         /* value already computed */
        return memo[n];

    /* extend the cached array to cover up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) return memo[memo_size - 1];      /* fallback on failure */
    memo = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = memo_size; i <= n; ++i) {
        /* use a wider type to avoid overflow on small seq_t types */
        unsigned long long sum = (unsigned long long)memo[i - 3]
                               + (unsigned long long)memo[i - 4];
        sum -= (unsigned long long)memo[i - 7];
        memo[i] = (seq_t)sum;
    }

    memo_size = new_cap;
    return memo[n];
}
