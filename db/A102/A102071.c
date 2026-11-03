/*
 * A102071  Pairwise sums of general ballot numbers (A002026).
 * https://oeis.org/A102071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n+5)*a(n-1)+(n-3)*a(n-2)-3*(n-3)*a(n-3))/(n+3)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=3, a(3)=7 */
#include <stdlib.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n)
{
    /* static cache */
    static seq_t *memo = NULL;   /* array indexed by n (1‑based) */
    static idx_t  capac = 0;     /* allocated capacity (last usable index) */
    static idx_t  up_to = 0;     /* largest n that has been computed */

    if (n < 1) return 0; /* undefined for n < 1, return 0 */

    /* Allocate / grow cache as needed */
    if (memo == NULL) {
        /* first allocation: at least up to n and 3 for base values */
        idx_t init_cap = 3;
        if (n > init_cap) init_cap = n;
        memo = mem_new(seq_t, (init_cap + 1)); /* index 0 unused */
        capac = init_cap;

        /* initialise base values */
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)3;
        memo[3] = (seq_t)7;
        up_to   = 3;
    } else if (n > capac) {
        /* grow by factor 2 until it can hold n */
        idx_t new_cap = capac;
        while (new_cap < n) {
            new_cap *= 2;
            if (new_cap < 4) new_cap = 4; /* minimal size */
        }
        seq_t *tmp = mem_resize(seq_t, memo, (new_cap + 1));
        memo = tmp;
        capac = new_cap;
        /* base values already present in the old array */
    }

    /* Compute missing values if necessary */
    if (up_to < n) {
        for (idx_t i = up_to + 1; i <= n; ++i) {
            /* recurrence: (n+3)*a_wide(n) = (3*n+5)*a_wide(n-1) + (n-3)*a_wide(n-2) - 3*(n-3)*a_wide(n-3) */
            seq_t an1 = memo[i-1];
            seq_t an2 = memo[i-2];
            seq_t an3 = memo[i-3];

            seq_t numer = (seq_t)(3*i+5) * an1
                        + (seq_t)(i-3) * an2
                        - (seq_t)3 * (seq_t)(i-3) * an3;

            seq_t denom = (seq_t)(i+3);
            memo[i] = numer / denom; /* exact division */
        }
        up_to = n;
    }

    return memo[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
