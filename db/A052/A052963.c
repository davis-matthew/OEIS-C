/*
 * A052963  a(0)=1, a(1)=2, a(2)=5, a(n) = 3*a(n+2) - a(n+3).
 * https://oeis.org/A052963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(n) = 3*a(n-1) - a(n-3) for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from a(n+3)=3*a(n+2)-a(n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for values already computed */
    static seq_t *memo = NULL;      /* array of terms */
    static idx_t memo_size = 0;     /* number of stored terms */
    static idx_t memo_cap  = 0;     /* allocated capacity */

    /* initialise the first three terms once */
    if (memo == NULL) {
        memo_cap  = 3;
        memo_size = 3;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (memo == NULL) {                 /* just to keep the function defined */
            return (seq_t)0;
        }
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)5;
    }

    /* if we already computed up to n, return it directly */
    if (n < memo_size)
        return memo[(size_t)n];

    /* extend storage if necessary */
    if (n + 1 > memo_cap) {
        idx_t new_cap = memo_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;                 /* memory failure; unlikely in test harness */
        memo      = tmp;
        memo_cap  = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = memo_size; i <= n; ++i) {
        /* use a wider intermediate type to avoid overflow before truncating */
        seq_t val = (seq_t)((unsigned long long)3 * memo[i - 1] - memo[i - 3]);
        memo[i] = val;
    }
    memo_size = n + 1;
    return memo[(size_t)n];
}
