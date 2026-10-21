/*
 * A087816  a(n) = a(a(n-1)) + a(n - 1 - a(n-1)) with a(1) = a(2) = 1.
 * https://oeis.org/A087816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a( a(n-1) ) + a( n-1-a(n-1) ) a(1) = a(2) = 1 Offset: 1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes for memory allocation */
#include <stdlib.h>

/* -------------------------------------------------------------
   a : compute the n-th term of OEIS A087816
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *buf = NULL;
    static idx_t capacity = 0;     /* allocated array size (>= n+1 when needed) */
    static idx_t known_max = 0;    /* highest index computed so far */

    /* ensure buffer is large enough for indices 1..n */
    if (capacity < n + 1) {
        idx_t new_cap = capacity ? capacity * 2 : 3;  /* at least 3 for a(1),a(2) */
        while (new_cap < n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* out of memory */
        /* initialise newly allocated slots to zero */
        for (idx_t i = capacity; i < new_cap; ++i)
            tmp[i] = 0;
        buf = tmp;
        capacity = new_cap;
    }

    /* base case: initialise a(1) and a(2) on first run or if not yet set */
    if (known_max < 2) {
        buf[0] = 0;            /* unused but safe for indexing */
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)1;
        known_max = 2;
    }

    /* iterate forward from known_max+1 to n, computing each term */
    /* note: a(n-1) is used as an index, and values stay within idx_t range for n≤10 000   */
    for (idx_t i = known_max + 1; i <= n; ++i) {
        idx_t prev_val = (idx_t)buf[i - 1];            /* a(i-1) */
        seq_t term1 = buf[prev_val];                   /* a( a(i-1) ) */
        idx_t j = (i - 1) - prev_val;                  /* n-1-a(n-1) */
        seq_t term2 = buf[j];                          /* a( n-1-a(n-1) ) */
        buf[i] = term1 + term2;
    }
    known_max = n;

    return buf[n];
}
