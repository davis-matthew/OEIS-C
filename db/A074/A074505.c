/*
 * A074505  a(n) = 1^n + 2^n + 9^n.
 * https://oeis.org/A074505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*a(n-1) – 18*a(n-2) + 8, a(0)=3, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* --- static memoisation table --- */
    static seq_t *memo = NULL;
    static size_t memo_len = 0;   /* number of terms that have been computed */
    static size_t memo_cap = 0;   /* allocated capacity in elements */

    /* initialise the table with the first two terms */
    if (memo_cap == 0) {
        memo_cap = 4;                                   /* spare room */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;                     /* allocation failed */
        memo[0] = (seq_t)3;
        memo[1] = (seq_t)12;
        memo_len = 2;
    }

    /* if the requested term is already known, return it */
    size_t nn = (size_t)n;
    if (nn < memo_len)
        return memo[nn];

    /* grow the memoisation array if necessary */
    if (nn + 1 > memo_cap) {
        size_t new_cap = memo_cap;
        while (nn + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                            /* allocation failed */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t i = memo_len; i <= nn; ++i) {
        seq_t term = memo[i-1] * 11;
        term -= memo[i-2] * 18;
        term += (seq_t)8;
        memo[i] = term;
    }
    memo_len = nn + 1;

    return memo[nn];
}
