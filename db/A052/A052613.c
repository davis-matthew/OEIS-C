/*
 * A052613  E.g.f. (1-2x)/(1-2x-x^2+x^3).
 * https://oeis.org/A052613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* 
 * Formula: a(0)=1, a(1)=0, a(2)=2.
 * For n ≥ 0 the sequence satisfies  
 *   a(n+3) = -(n^3+6n^2+11n+6)·a(n) + (n^2+5n+6)·a(n+1) + (2n+6)·a(n+2).
 * The function below computes a(n) for arbitrary n using this recurrence
 * and memoises computed values for future calls.  The computation uses only
 * integer arithmetic; for arbitrary‑precision builds seq_t has the needed
 * precision automatically.
 */
seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *memo = NULL;
    static size_t  cap  = 0;   /* allocated size */
    static idx_t  last = 0;    /* highest index computed */

    /* initialise table for the first three terms */
    if (memo == NULL) {
        cap  = 4;                          /* indices 0..3 */
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;        /* allocation failed */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)0;
        memo[2] = (seq_t)2;
        last    = 2;
    }

    /* quick return if value already known */
    if (n <= last) {
        return memo[(idx_t)n];
    }

    /* enlarge memoisation table if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cap) {
        size_t new_cap = cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_mem) return (seq_t)0;     /* allocation failed */
        for (size_t i = 0; i < cap; ++i) new_mem[i] = memo[i];
        free(memo);
        memo = new_mem;
        cap  = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        idx_t m = k - 3;              /* m corresponds to 'n' in the recurrence */
        seq_t mm = (seq_t)m;
        seq_t c1 = mm*mm*mm + (seq_t)6*mm*mm + (seq_t)11*mm + (seq_t)6;
        seq_t c2 = mm*mm + (seq_t)5*mm + (seq_t)6;
        seq_t c3 = (seq_t)2*mm + (seq_t)6;

        seq_t t1 = memo[m]   * c1;
        seq_t t2 = memo[m+1] * c2;
        seq_t t3 = memo[m+2] * c3;

        memo[k] = t2 + t3 - t1;     /* a(k) = -(c1)*a(m) + c2*a(m+1)+c3*a(m+2) */
    }

    last = n;
    return memo[n];
}
