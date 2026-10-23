/*
 * A025165  a(n) = H_n(1) / 2^floor(n/2) where H_n is the n-th Hermite
 *          polynomial.
 * https://oeis.org/A025165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 21, i64 n <= 34, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025165: a(n) = H_n(1) / 2^floor(n/2)
   where H_n(x) is the Hermite polynomial.
   Recurrence for a(n):
        a(0)=1, a(1)=2
        for n≥1,
            if (n-1) is even: a(n) = 2*a(n-1) - (n-1)*a(n-2)
            else            : a(n) =   a(n-1) - (n-1)*a(n-2)
   (derived from H_{n+1}=2H_n-2nH_{n-1} at x=1).  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* static memoisation state */
    static seq_t *memo = NULL;
    static size_t memo_len = 0;
    static idx_t  computed_max = 1;      /* highest index already computed */

    /* initialise memo array on first call */
    if (memo == NULL) {
        memo_len = 4;                   /* initial capacity */
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        if (!memo) return 0;            /* allocation failure unlikely */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
    }

    /* expand memo array if needed */
    if ((size_t)n + 1 > memo_len) {
        size_t new_len = memo_len;
        while ((size_t)n + 1 > new_len) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure unlikely */
        memo = tmp;
        memo_len = new_len;
    }

    /* if n already computed, return */
    if (n <= computed_max)
        return memo[n];

    /* compute a(k) for k from computed_max+1 to n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        idx_t prev = k - 1;            /* index used in recurrence */
        seq_t term = memo[prev]     /* a(prev) */
                   - (seq_t)prev * memo[prev - 1];  /* (prev)*a(prev-1) */

        if ((prev & 1) == 0)          /* prev even -> extra  a(prev)  */
            term += memo[prev];       /* should be 2*a(prev) - prev*a(prev-1) */

        memo[k] = term;
    }

    computed_max = n;
    return memo[n];
}
