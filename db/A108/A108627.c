/*
 * A108627  Logarithmic g.f.: Sum_{n>=1} a(n)/n*x^n = log(G108626(x)), where
 *          G108626(x) is g.f. for A108626.
 * https://oeis.org/A108627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - a(n-4) for n >= 5 a(1)=2, a(2)=6,
 *            a(3)=20, a(4)=66 (this follows from the generating function A(x)
 *            = 2*x*(1 - x - x^3)/(1 - 4*x + 2*x^2 + x^4) )
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the nth term of A108627.  The first term is a(1).  */
seq_t a(idx_t n)
{
    /* Static table of already computed values.
       memo[0] is unused; indices 1..last hold a(1)..a(last). */
    static seq_t *memo = NULL;
    static size_t cap = 0;      /* allocated capacity  (>= last+1) */
    static idx_t  last = 0;     /* highest n for which memo[n] is valid */

    if (n == 0)
        return (seq_t)0;       /* offset starts at 1; 0 is unused */

    /* Grow the table if necessary.  Use doubling for amortised O(1). */
    if (last < (idx_t)cap) {
        /* nothing to do */
    } else if (cap == 0) {
        cap = 8;
    } else {
        while (cap < (size_t)n)
            cap <<= 1;
    }
    if (cap < (size_t)n) cap = n;          /* final safety */
    if (memo == NULL || cap + 1 > cap) {
        seq_t *tmp = (seq_t *)realloc(memo, (cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure – unlikely */
        memo = tmp;
        /* initialise new elements to 0 */
        for (size_t i = last + 1; i <= cap; ++i)
            memo[i] = 0;
    }

    /* Initialise the first four terms once */
    if (last == 0) {
        memo[0] = 0;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)6;
        memo[3] = (seq_t)20;
        memo[4] = (seq_t)66;
        last = 4;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)                val = (seq_t)2;
        else if (i == 2)           val = (seq_t)6;
        else if (i == 3)           val = (seq_t)20;
        else if (i == 4)           val = (seq_t)66;
        else {
            seq_t t1 = memo[i - 1] * (seq_t)4;
            seq_t t2 = memo[i - 2] * (seq_t)2;
            seq_t t3 = memo[i - 4];
            val = t1 - t2 - t3;
        }
        memo[i] = val;
    }
    last = n;
    return memo[n];
}
