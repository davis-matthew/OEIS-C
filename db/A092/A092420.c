/*
 * A092420  a(n+2) = 9*a(n+1) - a(n) + 1, with a(1)=1, a(2)=10.
 * https://oeis.org/A092420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 9*a(n+1) – a(n) + 1, with initial values a(1) = 1, a(2)
 *            = 10
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Return base cases immediately */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;

    /* Memoisation of already computed values */
    static size_t memo_len  = 0;   /* number of terms currently stored */
    static size_t memo_cap  = 0;   /* allocated capacity of the array   */
    static seq_t *memo      = NULL;/* array storing a(1), a(2), ...    */

    /* Initialise storage on first call */
    if (memo == NULL) {
        memo_cap = 4;                           /* initial capacity */
        memo     = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0]  = (seq_t)1;                    /* a(1) */
        memo[1]  = (seq_t)10;                   /* a(2) */
        memo_len = 2;
    }

    /* Compute missing terms up to the requested index */
    size_t target = (size_t)n;                     /* n is assumed to fit in size_t */
    if (target > memo_len) {
        /* Grow the array if necessary */
        if (target > memo_cap) {
            while (memo_cap < target) memo_cap <<= 1;
            memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
        }

        /* Unroll recurrence: a(k) = 9*a(k-1) – a(k-2) + 1 */
        for (size_t i = memo_len; i < target; ++i) {
            seq_t a_nm1 = memo[i - 1];  /* a(k-1) */
            seq_t a_nm2 = memo[i - 2];  /* a(k-2) */
            memo[i] = 9 * a_nm1 - a_nm2 + 1;
        }
        memo_len = target;
    }

    return memo[n - 1];
}
