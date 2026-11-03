/*
 * A092634  a(n) = 1 - Sum_{k=2..n} k*k!.
 * https://oeis.org/A092634
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3 - (n+1)! (offset 2)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 406 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Memoised factorial table.  fact[i] holds i! for i >= 0. */
    static seq_t *fact = NULL;       /* pointer to factorial array */
    static idx_t fact_len = 0;       /* current array length (number of stored factorials) */

    /* initialise the table with 0! and 1! on first call */
    if (fact == NULL) {
        fact_len = 2;                            /* hold indices 0 and 1 */
        fact = mem_new(seq_t, fact_len);
        fact[0] = (seq_t)1;   /* 0! = 1 */
        fact[1] = (seq_t)1;   /* 1! = 1 */
    }

    /* we need factorial of (n+1) */
    idx_t need = n + 1;

    if (need >= fact_len) {
        /* enlarge the array to accommodate need */
        idx_t new_len = need + 1;                 /* indices 0..need inclusive */
        seq_t *tmp = mem_resize(seq_t, fact, new_len);
        if (!tmp) return (seq_t)0;                /* out of memory: return zero as a safe fallback */
        fact = tmp;
        /* compute missing factorials */
        for (idx_t i = fact_len; i <= need; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;     /* i! = (i-1)! * i */
        fact_len = new_len;
    }

    return (seq_t)3 - fact[need];
}
